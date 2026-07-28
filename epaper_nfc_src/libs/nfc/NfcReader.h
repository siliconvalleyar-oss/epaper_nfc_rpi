#pragma once

#include <nfc/nfc.h>
#include <cstdint>
#include <string>
#include <vector>

namespace NFC {

struct NfcTag {
    std::vector<uint8_t> uid;
    uint8_t type;
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
