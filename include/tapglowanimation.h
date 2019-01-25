#pragma once

#include <thread>

#include <libevdev-1.0/libevdev/libevdev.h>

#include "g513.h"
#include "safequeue.h"

struct KeyTime {
	G513::Key key;
	int32_t tick;
};

class TapGlowAnimation {
	G513 kb;
	G513::Key keyMap[128] { G513::Key::undef, G513::Key::esc, G513::Key::n1, G513::Key::n2, G513::Key::n3, G513::Key::n4, G513::Key::n5, G513::Key::n6, G513::Key::n7, G513::Key::n8, G513::Key::n9, G513::Key::n0, G513::Key::minus, G513::Key::equal, G513::Key::backspace, G513::Key::tab, G513::Key::q, G513::Key::w, G513::Key::e, G513::Key::r, G513::Key::t, G513::Key::y, G513::Key::u, G513::Key::i, G513::Key::o, G513::Key::p, G513::Key::open_bracket, G513::Key::close_bracket, G513::Key::enter, G513::Key::ctrl_left, G513::Key::a, G513::Key::s, G513::Key::d, G513::Key::f, G513::Key::g, G513::Key::h, G513::Key::j, G513::Key::k, G513::Key::l, G513::Key::semicolon, G513::Key::quote, G513::Key::tilde, G513::Key::shift_left, G513::Key::backslash, G513::Key::z, G513::Key::x, G513::Key::c, G513::Key::v, G513::Key::b, G513::Key::n, G513::Key::m, G513::Key::comma, G513::Key::period, G513::Key::slash, G513::Key::shift_right, G513::Key::num_asterisk, G513::Key::alt_left, G513::Key::space, G513::Key::caps_lock, G513::Key::f1, G513::Key::f2, G513::Key::f3, G513::Key::f4, G513::Key::f5, G513::Key::f6, G513::Key::f7, G513::Key::f8, G513::Key::f9, G513::Key::f10, G513::Key::num_lock, G513::Key::scroll_lock, G513::Key::num_7, G513::Key::num_8, G513::Key::num_9, G513::Key::num_minus, G513::Key::num_4, G513::Key::num_5, G513::Key::num_6, G513::Key::num_plus, G513::Key::num_1, G513::Key::num_2, G513::Key::num_3, G513::Key::num_0, G513::Key::num_dot, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::f11, G513::Key::f12, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::num_enter, G513::Key::ctrl_right, G513::Key::num_slash, G513::Key::print_screen, G513::Key::alt_right, G513::Key::undef, G513::Key::home, G513::Key::arrow_top, G513::Key::page_up, G513::Key::arrow_left, G513::Key::arrow_right, G513::Key::end, G513::Key::arrow_bottom, G513::Key::page_down, G513::Key::insert, G513::Key::del, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::pause_break, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::undef, G513::Key::win_left, G513::Key::undef, G513::Key::menu };
	int keyPresses[256] {};

	const G513::Color SLEEP_COLOR { 0x00, 0x00, 0x00 };
	const G513::Color IDLE_COLOR { 0x00, 0x90, 0x90 };
	const G513::Color GLOW_COLOR { 0x90, 0x00, 0x00 };
	const int32_t GLOW_DURATION = 3000;
	const int32_t SLEEP_TIME = 20 * 60 * 1000;

	libevdev* device = nullptr;

	std::thread glowThread;
	std::thread sleepThread;
	SafeQueue<KeyTime> queue;
	std::mutex m;
	std::condition_variable sleepCv;
	bool enabled;
	bool running;
	bool sleeping;
	int32_t lastKeyTap;

	void sleepWorker();

	void glowWorker();

	int32_t getTick();

public:
	TapGlowAnimation();

	~TapGlowAnimation();

	void init();

	void startAnimation();

	void stop();

	void mainLoop();
};
