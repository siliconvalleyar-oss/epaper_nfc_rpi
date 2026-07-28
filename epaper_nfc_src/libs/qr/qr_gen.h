#pragma once
#include <nfc/nfc.h>
#include <cstdint>

extern "C" {
#include <qrencode.h>
}

namespace QR {

constexpr int QrBufferSize = 296 * 152;

struct QrGen {
    uint8_t imageBuffer[QrBufferSize] = {0};

    void clear() { memset(imageBuffer, 0, sizeof(imageBuffer)); }
    void drawQRCode(const char* data, int scale = 4);
    int qr_generator();
};

}
