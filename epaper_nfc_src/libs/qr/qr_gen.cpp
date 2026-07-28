#include <qr/qr_gen.h>
#include <cstring>
#include <iostream>

namespace QR {

void QrGen::drawQRCode(const char* data, int scale) {
    clear();

    QRcode* qrcode = QRcode_encodeString(data, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qrcode) {
        std::cerr << "[QR] Error al generar QR" << std::endl;
        return;
    }

    int qrSize = qrcode->width;
    int xOffset = (296 - qrSize * scale) / 2;
    int yOffset = (152 - qrSize * scale) / 2;

    for (int y = 0; y < qrSize; y++) {
        for (int x = 0; x < qrSize; x++) {
            bool isBlack = qrcode->data[y * qrSize + x] & 0x01;
            for (int dy = 0; dy < scale; dy++) {
                for (int dx = 0; dx < scale; dx++) {
                    int px = xOffset + x * scale + dx;
                    int py = yOffset + y * scale + dy;
                    if (px < 0 || px >= 296 || py < 0 || py >= 152) continue;
                    int byteIndex = (py * 296 + px) / 8;
                    int bitIndex = 7 - (px % 8);
                    if (isBlack) {
                        imageBuffer[byteIndex] |= (1 << bitIndex);
                    } else {
                        imageBuffer[byteIndex] &= ~(1 << bitIndex);
                    }
                }
            }
        }
    }

    QRcode_free(qrcode);
}

int QrGen::qr_generator() {
    const char* data = "WIFI:T:WPA;S:SSID;P:Password;;";
    drawQRCode(data, 4);
    return 0;
}

}
