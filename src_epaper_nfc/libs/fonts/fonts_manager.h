#pragma once

#include <string>
#include <cstdint>

enum FontType {
    FONT_5x8 = 0,
    FONT_7x8_THICK,
    FONT_8x8_WIDE,
    FONT_3x8_TINY
};

class FontManager {
private:
    struct FontInfo {
        FontType type;
        const void* data;
        uint8_t width;
        uint8_t height;
        uint8_t start_char;
        uint8_t end_char;
        const char* name;
    } currentFont;

    const void* getFontData(FontType type) const;
    uint8_t getCharSize(FontType type) const;

public:
    FontManager();
    void setFont(FontType type);
    const uint8_t* getCharBitmap(char c) const;
    uint8_t getFontWidth() const { return currentFont.width; }
    uint8_t getFontHeight() const { return currentFont.height; }
    FontType getCurrentFontType() const { return currentFont.type; }
};


