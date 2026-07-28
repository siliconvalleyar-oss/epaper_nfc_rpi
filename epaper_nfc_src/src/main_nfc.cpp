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

            std::cout << "Tag detectado - UID: " << uid
                      << " | Tipo: 0x" << std::hex << (int)tag.type << std::dec
                      << " | Bytes UID: " << (int)tag.uid.size() << std::endl;

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
