#pragma once
#include <epaper/epaper.h>
#include <string>

namespace EPAPER {

const pins_t boardRaspberryPi = {
    .panelBusy        = 25,    // GPIO25 - Pin 22
    .panelDC          = 24,    // GPIO24 - Pin 18
    .panelReset       = 23,    // GPIO23 - Pin 16
    .panelCS          = 27,    // GPIO27 - Pin 13
    .panelON_EXT2     = NOT_CONNECTED,
    .panelSPI43_EXT2  = NOT_CONNECTED,
    .flashCS          = 22     // GPIO22 - Pin 15
};

inline const pins_t& getBoardConfig(const std::string& model = "RaspberryPi") {
    return boardRaspberryPi;
}

} // namespace EPAPER
