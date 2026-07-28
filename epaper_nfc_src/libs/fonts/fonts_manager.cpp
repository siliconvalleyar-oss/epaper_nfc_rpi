#include <fonts/fonts.h>
#include <fonts/fonts_manager.h>
#include <cstring>

const void* FontManager::getFontData(FontType type) const {
    switch(type) {
        case FONT_5x8: return Font_One;
        case FONT_7x8_THICK: return Font_Two;
        case FONT_8x8_WIDE: return Font_Four;
        case FONT_3x8_TINY: return Font_Five;
        default: return Font_One;
    }
}

uint8_t FontManager::getCharSize(FontType type) const {
    switch(type) {
        case FONT_5x8: return 5;
        case FONT_7x8_THICK: return 7;
        case FONT_8x8_WIDE: return 8;
        case FONT_3x8_TINY: return 3;
        default: return 5;
    }
}

FontManager::FontManager() {
    currentFont.type = FONT_5x8;
    currentFont.data = Font_One;
    currentFont.width = 5;
    currentFont.height = 8;
    currentFont.start_char = 0;
    currentFont.end_char = 127;
    currentFont.name = "5x8";
}

void FontManager::setFont(FontType type) {
    currentFont.type = type;
    currentFont.data = getFontData(type);

    switch(type) {
        case FONT_5x8:
            currentFont.width = 5;
            currentFont.height = 8;
            currentFont.start_char = 0;
            currentFont.end_char = 127;
            currentFont.name = "5x8";
            break;
        case FONT_7x8_THICK:
            currentFont.width = 7;
            currentFont.height = 8;
            currentFont.start_char = 32;
            currentFont.end_char = 127;
            currentFont.name = "7x8 Thick";
            break;
        case FONT_8x8_WIDE:
            currentFont.width = 8;
            currentFont.height = 8;
            currentFont.start_char = 32;
            currentFont.end_char = 90;
            currentFont.name = "8x8 Wide";
            break;
        case FONT_3x8_TINY:
            currentFont.width = 3;
            currentFont.height = 8;
            currentFont.start_char = 32;
            currentFont.end_char = 126;
            currentFont.name = "3x8 Tiny";
            break;
    }
}

const uint8_t* FontManager::getCharBitmap(char c) const {
    int index = (int)c - currentFont.start_char;
    if (index < 0 || index > (currentFont.end_char - currentFont.start_char)) {
        return nullptr;
    }

    size_t charSize = getCharSize(currentFont.type);
    const uint8_t* fontData = static_cast<const uint8_t*>(currentFont.data);
    return &fontData[index * charSize];
}
