#include <nfc/nfc.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

namespace NFC {

NfcReader::NfcReader(const char* device) : m_fd(-1) {
    snprintf(m_device, sizeof(m_device), "%s", device);
}

NfcReader::~NfcReader() {
    close();
}

bool NfcReader::open(speed_t baud) {
    m_fd = ::open(m_device, O_RDWR | O_NOCTTY | O_SYNC);
    if (m_fd < 0) {
        perror("ERROR opening NFC device");
        return false;
    }

    struct termios options;
    if (tcgetattr(m_fd, &options) != 0) {
        perror("ERROR getting tty attributes");
        close();
        return false;
    }

    cfmakeraw(&options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    if (tcsetattr(m_fd, TCSANOW, &options) != 0) {
        perror("ERROR setting tty attributes");
        close();
        return false;
    }

    tcflush(m_fd, TCIOFLUSH);
    return true;
}

void NfcReader::close() {
    if (m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }
}

bool NfcReader::init() {
    if (!isOpen()) return false;

    uint8_t cmd[] = { 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    cmd[1] = 0x01; // SAMConfiguration
    cmd[2] = 0x00; // Normal mode
    cmd[3] = 0x00; // Timeout
    cmd[4] = 0x00; // IRQ

    uint8_t len = 14;
    cmd[0] = 0x55;
    uint8_t lcs = (256 - (len + 1)) & 0xFF;
    cmd[len + 1] = lcs;
    cmd[len + 2] = 0x00;

    if (!sendCommand(cmd, len + 3)) return false;

    uint8_t resp[16];
    size_t respLen = 0;
    return readResponse(resp, sizeof(resp), respLen, 500);
}

bool NfcReader::sendCommand(const uint8_t* cmd, size_t len) {
    if (!isOpen()) return false;
    if (write(m_fd, cmd, len) != (ssize_t)len) {
        perror("ERROR writing to NFC");
        return false;
    }
    tcdrain(m_fd);
    return true;
}

bool NfcReader::readResponse(uint8_t* buf, size_t maxLen, size_t& outLen, int timeoutMs) {
    if (!isOpen()) return false;

    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(m_fd, &fds);
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;

    int ret = select(m_fd + 1, &fds, nullptr, nullptr, &tv);
    if (ret <= 0) return false;

    ssize_t n = read(m_fd, buf, maxLen);
    if (n <= 0) return false;

    outLen = (size_t)n;
    return true;
}

uint8_t NfcReader::checksum(const uint8_t* buf, size_t len) const {
    uint8_t sum = 0;
    for (size_t i = 0; i < len; i++) sum ^= buf[i];
    return sum;
}

bool NfcReader::poll(NfcTag& outTag) {
    if (!isOpen()) return false;

    uint8_t cmd[11];
    cmd[0] = 0x55;
    cmd[1] = 0x4A; // InListPassiveTarget
    cmd[2] = 0x01; // 1 target max
    cmd[3] = 0x00; // 106A

    cmd[4] = 0x00; // D5
    cmd[5] = 0x00; // len low
    cmd[6] = 0x00; // len high
    cmd[7] = 0x00; // TFI

    uint8_t lcs = (uint8_t)(256 - ((cmd[1] + cmd[2] + cmd[3]) % 256));
    cmd[8] = lcs;
    cmd[9] = 0x00;
    cmd[10] = 0x00;

    if (!sendCommand(cmd, 11)) return false;

    uint8_t buf[64];
    size_t len = 0;
    if (!readResponse(buf, sizeof(buf), len, 500)) return false;
    if (len < 6) return false;

    if (buf[0] != 0x55 || buf[1] != 0xD5) return false;

    outTag.uid.clear();

    uint8_t nbTargets = buf[4];
    if (nbTargets != 0x01) return false;

    uint8_t uidLen = buf[7];
    outTag.type = buf[6];
    outTag.uid.assign(buf + 8, buf + 8 + uidLen);

    return true;
}

}
