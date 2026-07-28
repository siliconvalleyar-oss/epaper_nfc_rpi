#include <iostream>
#include <memory>
#include <cstring>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <epaper/epaper_display.h>
#include <app/config.h>
#include <nfc/nfc.h>

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

int main() {
    signal(SIGINT, onSignal);
    signal(SIGTERM, onSignal);

    std::cout << "=== NFC → E-Paper Reader ===" << std::endl;

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

    NFC::NfcReader nfc("/dev/ttyS0");
    if (!nfc.open()) {
        std::cerr << "WARNING: NFC device open failed, retrying /dev/ttyAMA0..." << std::endl;
        nfc = NFC::NfcReader("/dev/ttyAMA0");
        if (!nfc.open()) {
            std::cerr << "WARNING: NFC device open failed, retrying /dev/serial0..." << std::endl;
            nfc = NFC::NfcReader("/dev/serial0");
            if (!nfc.open()) {
                std::cerr << "ERROR: Cannot open NFC device (PN532 UART)." << std::endl;
                std::cerr << "Check wiring: TXD->RXD, RXD->TXD, GND->GND, VCC->3.3V" << std::endl;
                return 1;
            }
        }
    }

    if (!nfc.init()) {
        std::cerr << "ERROR: NFC init failed" << std::endl;
        return 1;
    }

    std::cout << "NFC reader ready. Waiting for tags..." << std::endl;

    bool lastTagPresent = false;
    std::string lastUid;
    int stableCount = 0;
    const int STABLE_REQUIRED = 3;

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

            if (uid == lastUid) {
                stableCount++;
            } else {
                stableCount = 0;
                lastUid = uid;
            }

            if (stableCount >= STABLE_REQUIRED) {
                std::string typeStr = (tag.type == 0x01) ? "Type A" : std::string("Type 0x") + std::to_string(tag.type);
                display->drawCenteredString(y, "Tag detectado:", FONT_5x8, true);
                y += 12;

                std::string uidDisplay = uid;
                if (uidDisplay.length() > 19) {
                    uidDisplay = uidDisplay.substr(0, 19);
                }

                display->drawCenteredString(y, uidDisplay, FONT_8x8_WIDE, true);
                y += 14;

                char info[64];
                snprintf(info, sizeof(info), "UID: %02zX bytes", tag.uid.size());
                display->drawCenteredString(y, info, FONT_5x8, true);
                y += 12;

                display->drawCenteredString(y, typeStr.c_str(), FONT_5x8, true);
                y += 12;

                if (stableCount > 10) {
                    display->drawCenteredString(y, "(retire la tarjeta)", FONT_3x8_TINY, true);
                    y += 9;
                }

                lastTagPresent = true;
            } else {
                display->drawCenteredString(y, "Leyendo...", FONT_5x8, true);
                y += 12;
            }
        } else {
            stableCount = 0;
            lastTagPresent = false;
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
