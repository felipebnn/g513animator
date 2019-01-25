#include "tapglowanimation.h"

#include <cstring>

#include <unistd.h>
#include <fcntl.h>

TapGlowAnimation::TapGlowAnimation() {
    lastKeyTap = getTick();
    char buffer[256];

    for (int i=0; ; ++i) {
        std::snprintf(buffer, 255, "/dev/input/event%d", i);
        int fd = open(buffer, O_RDONLY);

        if (fd < 0) {
            if (i >= 32) {
                std::cerr << "Could not find the right keyboard..." << std::endl;
                exit(-1);
            }
        } else {
            if (libevdev_new_from_fd(fd, &device) >= 0) {
                if (!strcmp("Logitech G513 Carbon Tactile", libevdev_get_name(device))) {
                    break;
                }
                libevdev_free(device);
            }

            close(fd);
        }
    }
}

TapGlowAnimation::~TapGlowAnimation() {
    stop();
}

void TapGlowAnimation::sleepWorker() {
    while (running) {
        std::unique_lock<std::mutex> lock(m);
        while (running && lastKeyTap + SLEEP_TIME > getTick()) {
            int32_t x = SLEEP_TIME - (getTick() - lastKeyTap);
            sleepCv.wait_for(lock, std::chrono::milliseconds(x));
        }
        sleeping = true;
        kb.setAllKeys(SLEEP_COLOR);
        sleepCv.wait(lock, [=] { return !running || !sleeping; });
    }
}

void TapGlowAnimation::glowWorker() {
    while (running) {
        KeyTime kt = queue.pop();
        if (kt.tick == 0) {
            break;
        }

        int32_t sleepTime = kt.tick - getTick();
        if (sleepTime >= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }

        std::lock_guard<std::mutex> lock(m);
        keyPresses[static_cast<int>(kt.key)] -= 1;

        if (keyPresses[static_cast<int>(kt.key)] <= 0) {
            kb.setKey(kt.key, IDLE_COLOR);
        }
    }
}

int32_t TapGlowAnimation::getTick() {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
}

void TapGlowAnimation::init() {
    lastKeyTap = getTick();
    enabled = true;
    running = true;

    startAnimation();

    glowThread = std::thread(&TapGlowAnimation::glowWorker, this);
    sleepThread = std::thread(&TapGlowAnimation::sleepWorker, this);

    kb.setAllKeys(IDLE_COLOR);

    mainLoop();
}

void TapGlowAnimation::startAnimation() {
    kb.setAllKeys(SLEEP_COLOR);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    kb.setAllKeys(IDLE_COLOR);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    kb.setAllKeys(SLEEP_COLOR);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void TapGlowAnimation::stop() {
    running = false;
    sleepCv.notify_one();
    queue.push(KeyTime{{}, 0});
    glowThread.join();
    sleepThread.join();
}

void TapGlowAnimation::mainLoop() {
    input_event ev;

    while (running) {
        if (!libevdev_next_event(device, LIBEVDEV_READ_FLAG_BLOCKING, &ev)) {
            // std::cout << libevdev_event_type_get_name(ev.type) << " | " << libevdev_event_code_get_name(ev.type, ev.code) << " | " << ev.value << std::endl;

            if (ev.type == 1) {
                // std::cout << ev.code << std::endl;
                G513::Key key = keyMap[ev.code];

                std::lock_guard<std::mutex> lock(m);

                lastKeyTap = getTick();
                if (sleeping) {
                    sleeping = false;
                    sleepCv.notify_one();
                    kb.setAllKeys(IDLE_COLOR, !enabled);
                }

                if (enabled || key == G513::Key::pause_break) {
                    if (ev.value == 1) {
                        kb.setKey(key, GLOW_COLOR);
                    } else if (ev.value == 0) {
                        queue.push(KeyTime{key, lastKeyTap + GLOW_DURATION});
                        keyPresses[static_cast<int>(key)] += 1;

                        if (key == G513::Key::pause_break) {	
                            enabled = !enabled;
                        }
                    }
                }
            }
        }
    }
}