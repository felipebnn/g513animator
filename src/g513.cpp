#include "g513.h"

#include <thread>

G513::G513() {
    m_hidHandle = hid_open(0x046d, 0xc33c, nullptr);

    if (!m_hidHandle) {
        std::cerr << "Could not find the device" << std::endl;
        exit(-1);
    }
}

bool G513::sendData(const std::vector<unsigned char>& data) {
    if (data.size() > 0) {
        if (hid_write(m_hidHandle, const_cast<unsigned char*>(data.data()), data.size()) < 0) {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return true;
}

void G513::setKey(G513::Key k, const G513::Color& c, bool commit) {
    keyData[9] = static_cast<uint8_t>(k);
    keyData[10] = c.red;
    keyData[11] = c.green;
    keyData[12] = c.blue;
    sendData(keyData);

    if (commit) {
        this->commit();
    }
}

void G513::setAllKeys(const G513::Color& c, bool commit) {
    std::vector<unsigned char> data = keyData;

    int j = 0;
    for (int i=1; i<256; ++i) {
        if (j > 13) {
            j = 0;
            sendData(data);
        }
        data[9 + j * 4] = i;
        data[10 + j * 4] = c.red;
        data[11 + j * 4] = c.green;
        data[12 + j * 4] = c.blue;
        ++j;
    }

    if (j) {
        sendData(data);
    }

    if (commit) {
        this->commit();
    }
}

void G513::commit() {
    sendData(commitData);
}
