#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

namespace NFC {

struct NfcTag {
    std::vector<uint8_t> uid;
    uint8_t type;
};

class NfcReader {
public:
    explicit NfcReader(const char* device = "/dev/ttyS0");
    ~NfcReader();

    bool open();
    void close();
    bool isOpen() const { return m_fd >= 0; }

    bool init();
    bool poll(NfcTag& outTag);

private:
    bool sendCommand(const uint8_t* cmd, size_t len);
    bool readResponse(uint8_t* buf, size_t maxLen, size_t& outLen, int timeoutMs = 1000);
    uint8_t checksum(const uint8_t* buf, size_t len) const;

    int m_fd;
    char m_device[64];
};

}
