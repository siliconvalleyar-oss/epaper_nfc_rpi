#include <iostream>
#include <memory>
#include <cstring>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <epaper/epaper_display.h>
#include <app/config.h>
#include <nfc/NfcReader.h>
#include <qr/qr_gen.h>

static volatile bool running = true;

static void onSignal(int) {
    running = false;
}

static std::string uidToHexString(const NFC::NfcTag& tag) {
    char buf[128] = {0};
    char* p = buf;
    for (size_t i = 0; i < tag.uid.size(); i++) {
        p += snprintf(p, sizeof(buf) - (p - buf), "%s%02X", (i > 0) ? ":" : "", tag.uid[i]);
    }
    return std::string(buf);
}

static std::string getTagTypeString(const NFC::NfcTag& tag) {
    uint16_t atqa16 = NFC::atqaToUint16(tag);

    if (atqa16 == 0x0004 && tag.sak == 0x08) {
        return "Mifare Classic 1K";
    } else if (atqa16 == 0x0044 && tag.sak == 0x00) {
        return "Mifare Classic 4K";
    } else if (atqa16 == 0x0044 && tag.sak == 0x40) {
        return "Mifare DESFire";
    } else if (atqa16 == 0x0004 && tag.sak == 0x18) {
        return "Mifare Pro X";
    } else if (atqa16 == 0x0004 && tag.sak == 0x00) {
        return "Mifare Classic 1K";
    } else if (atqa16 == 0x0208) {
        return "Mifare Ultralight";
    } else if (atqa16 == 0x4400) {
        return "Mifare DESFire EV1";
    }

    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "ATQA:%04X SAK:%02X", atqa16, tag.sak);
    return std::string(buf);
}

int main() {
    signal(SIGINT, onSignal);
    signal(SIGTERM, onSignal);

    std::cout << "=== NFC → E-Paper Reader v" << VERSION_STRING << " ===" << std::endl;

    if (!bcm2835_init()) {
        std::cerr << "ERROR: bcm2835_init() failed" << std::endl;
        return 1;
    }

    auto display = std::make_unique<EPAPER_DISPLAY::EpaperDisplay>(
        eScreen_EPD_266, EPAPER::boardRaspberryPi
    );
    if (!display->init()) {
        std::cerr << "ERROR: display init failed" << std::endl;
        return 1;
    }

    NFC::NfcReader nfc;
    if (!nfc.open()) {
        std::cerr << "ERROR: Cannot open NFC device (PN532)." << std::endl;
        return 1;
    }

    display->clearScreen(true);
    display->drawCenteredString(20, "EPAPER + NFC", FONT_7x8_THICK, true);
    display->drawCenteredString(35, "v" VERSION_STRING, FONT_3x8_TINY, true);
    display->drawCenteredString(50, "Raspberry Pi", FONT_5x8, true);
    display->drawLine(20, 58, 265, 58, true);

    display->drawString(20, 70, "Display:", FONT_5x8, true);
    display->drawString(20, 84, "Modelo: EPD_2.66", FONT_3x8_TINY, true);
    display->drawString(20, 96, "Panel: B/W/R (3col)", FONT_3x8_TINY, true);
    display->drawString(20, 110, "SPI: activo", FONT_3x8_TINY, true);
    display->drawString(20, 122, "pines: RST=23, DC=24, CS=27, BUSY=25", FONT_3x8_TINY, true);

    display->drawLine(20, 138, 265, 138, true);
    display->drawString(20, 150, "NFC Reader:", FONT_5x8, true);

    if (nfc.isOpen()) {
        display->drawString(20, 166, "Estado: OPEN", FONT_3x8_TINY, true);
        display->drawString(20, 178, "Puerto: UART", FONT_3x8_TINY, true);
    } else {
        display->drawString(20, 166, "Estado: INIT...", FONT_3x8_TINY, true);
        display->drawString(20, 178, "Puerto: /dev/ttyS0", FONT_3x8_TINY, true);
    }

    display->drawCenteredString(210, "Iniciando sistema...", FONT_3x8_TINY, true);
    display->update();
    usleep(3000000);

    display->clearScreen(true);
    display->drawCenteredString(70, "SISTEMA LISTO", FONT_7x8_THICK, true);
    display->drawCenteredString(100, "Esperando tag NFC...", FONT_5x8, true);
    display->update();
    usleep(800000);

    std::cout << "NFC reader ready. Waiting for tags..." << std::endl;

    std::string lastUid;
    int stableCount = 0;
    const int STABLE_REQUIRED = 3;
    int totalUniqueTags = 0;
    time_t tagFirstSeen = 0;
    std::string displayedUid;

    while (running) {
        display->clearScreen(true);

        int y = 10;
        display->drawCenteredString(y, "NFC READER", FONT_7x8_THICK, true);
        y += 14;
        display->drawLine(10, y, 285, y, true);
        y += 10;

        NFC::NfcTag tag;
        bool tagPresent = nfc.poll(tag);

        if (tagPresent) {
            std::string uid = uidToHexString(tag);
            std::string typeStr = getTagTypeString(tag);

            std::cout << "Tag detectado - UID: " << uid
                      << " | ATQA: " << std::hex << (int)tag.atqa[0] << ":" << (int)tag.atqa[1] << std::dec
                      << " | SAK: 0x" << std::hex << (int)tag.sak << std::dec
                      << " | Tipo: " << typeStr
                      << " | Bytes UID: " << (int)tag.uid.size() << std::endl;

            if (uid == lastUid) {
                stableCount++;
            } else {
                stableCount = 0;
                lastUid = uid;
            }

            if (stableCount >= STABLE_REQUIRED) {
                if (uid != displayedUid) {
                    displayedUid = uid;
                    if (tagFirstSeen == 0) {
                        tagFirstSeen = time(nullptr);
                        totalUniqueTags++;
                    }
                }

                display->drawCenteredString(y, "Tag detectado:", FONT_5x8, true);
                y += 12;

                std::string uidDisplay = uid;
                if (uidDisplay.length() > 19) {
                    uidDisplay = uidDisplay.substr(0, 19);
                }
                display->drawCenteredString(y, uidDisplay, FONT_8x8_WIDE, true);
                y += 14;

                display->drawCenteredString(y, typeStr.c_str(), FONT_5x8, true);
                y += 12;

                char info[64];
                snprintf(info, sizeof(info), "UID: %d bytes", (int)tag.uid.size());
                display->drawCenteredString(y, info, FONT_5x8, true);
                y += 10;

                snprintf(info, sizeof(info), "ATQA:%02X:%02X SAK:%02X", tag.atqa[0], tag.atqa[1], tag.sak);
                display->drawCenteredString(y, info, FONT_3x8_TINY, true);
                y += 9;

                int elapsed = (int)(time(nullptr) - tagFirstSeen);
                if (elapsed >= 60) {
                    snprintf(info, sizeof(info), "Tiempo: %dm%ds", elapsed / 60, elapsed % 60);
                } else {
                    snprintf(info, sizeof(info), "Tiempo: %ds", elapsed);
                }
                display->drawCenteredString(y, info, FONT_3x8_TINY, true);
                y += 9;

                snprintf(info, sizeof(info), "Lecturas: %d", stableCount);
                display->drawCenteredString(y, info, FONT_3x8_TINY, true);
                y += 9;

                QR::QrGen qr;
                qr.drawUID(uid, 3);
                int qrStartX = 175;
                int qrStartY = 20;
                for (int row = 0; row < QR::QrGen::HEIGHT; row++) {
                    for (int col = 0; col < QR::QrGen::WIDTH; col++) {
                        if (qr.image[row][col]) {
                            display->drawPixel(qrStartX + col, qrStartY + row, true);
                        }
                    }
                }

                if (stableCount > 10) {
                    display->drawCenteredString(y, "(retire la tarjeta)", FONT_3x8_TINY, true);
                    y += 9;
                }
            } else {
                display->drawCenteredString(y, "Leyendo...", FONT_5x8, true);
                y += 12;
            }
        } else {
            stableCount = 0;
            lastUid.clear();

            display->drawCenteredString(y, "Acerca una tarjeta NFC", FONT_5x8, true);
            y += 12;
            display->drawCenteredString(y, "o tag al lector PN532", FONT_5x8, true);
            y += 12;
            display->drawCenteredString(y, "", FONT_5x8, true);
            y += 12;
            display->drawCenteredString(y, "Esperando...", FONT_3x8_TINY, true);
        }

        display->update();
        usleep(200000);
    }

    std::cout << "Shutting down..." << std::endl;
    display.reset();
    bcm2835_close();
    std::cout << "Done." << std::endl;
    return 0;
}
