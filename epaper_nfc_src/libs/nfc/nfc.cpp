#include <nfc/nfc.h>
#include "nfc.h"
#include <iostream>

namespace NFC {

NfcReader::NfcReader() : m_context(nullptr), m_device(nullptr) {
}

NfcReader::~NfcReader() {
    close();
}

bool NfcReader::open() {
    if (nfc_init(&m_context) < 0) {
        std::cerr << "ERROR: nfc_init failed" << std::endl;
        m_context = nullptr;
        return false;
    }

    m_device = nfc_open(m_context, nullptr);
    if (!m_device) {
        std::cerr << "ERROR: nfc_open failed - no PN532 detected" << std::endl;
        nfc_exit(m_context);
        m_context = nullptr;
        return false;
    }

    if (nfc_initiator_init(m_device) < 0) {
        std::cerr << "ERROR: nfc_initiator_init failed" << std::endl;
        nfc_close(m_device);
        nfc_exit(m_context);
        m_device = nullptr;
        m_context = nullptr;
        return false;
    }

    std::cout << "NFC device opened successfully" << std::endl;
    return true;
}

void NfcReader::close() {
    if (m_device) {
        nfc_close(m_device);
        m_device = nullptr;
    }
    if (m_context) {
        nfc_exit(m_context);
        m_context = nullptr;
    }
}

bool NfcReader::poll(NfcTag& outTag) {
    if (!isOpen()) return false;

    nfc_modulation nm;
    nm.nmt = NMT_ISO14443A;
    nm.nbr = NBR_106;

    nfc_target nt;
    if (nfc_initiator_select_passive_target(m_device, nm, nullptr, 0, &nt) > 0) {
        outTag.uid.clear();
        outTag.type = 0x01;
        outTag.uid.assign(
            nt.nti.nai.abtUid,
            nt.nti.nai.abtUid + nt.nti.nai.szUidLen
        );
        return true;
    }

    return false;
}

}
