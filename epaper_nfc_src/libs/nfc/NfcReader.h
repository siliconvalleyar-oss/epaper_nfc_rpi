#pragma once

#include <nfc/nfc.h>
#include <cstdint>
#include <string>
#include <vector>

namespace NFC {

struct NfcTag {
    std::vector<uint8_t> uid;
    uint8_t type;
    uint8_t atqa[2];
    uint8_t sak;
};

inline uint16_t atqaToUint16(const NFC::NfcTag& tag) {
    return (uint16_t)(((uint16_t)tag.atqa[0] << 8) | tag.atqa[1]);
}

struct TagInfo {
    NFC::NfcTag tag;
    int readCount;
    time_t firstSeen;
    time_t lastSeen;
};

class NfcReader {
public:
    NfcReader();
    ~NfcReader();

    NfcReader(const NfcReader&) = delete;
    NfcReader& operator=(const NfcReader&) = delete;

    bool open();
    void close();
    bool isOpen() const { return m_device != nullptr; }

    bool poll(NfcTag& outTag);

private:
    nfc_context* m_context;
    nfc_device* m_device;
};

}
