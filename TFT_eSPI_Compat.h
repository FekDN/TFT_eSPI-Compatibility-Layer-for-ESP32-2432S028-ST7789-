// Copyright (c) 2026 Dmitry Feklin (FeklinDN@gmail.com) Apache License 2.0.
// TFT_eSPI_Compat.h
// TFT_eSPI library API compatibility layer based on LovyanGFX.
// Fully configured for the Sunton ESP32-2432S028 board (ST7789 display, XPT2046 touch).

#pragma once

// Install the LovyanGFX library via the Arduino IDE library manager or PlatformIO.
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// === Color constants (RGB565), full compatibility with TFT_eSPI ===
#define TFT_BLACK       0x0000
#define TFT_NAVY        0x000F
#define TFT_DARKGREEN   0x03E0
#define TFT_DARKCYAN    0x03EF
#define TFT_MAROON      0x7800
#define TFT_PURPLE      0x780F
#define TFT_OLIVE       0x7BE0
#define TFT_LIGHTGREY   0xD69A
#define TFT_DARKGREY    0x7BEF
#define TFT_BLUE        0x001F
#define TFT_GREEN       0x07E0
#define TFT_CYAN        0x07FF
#define TFT_RED         0xF800
#define TFT_MAGENTA     0xF81F
#define TFT_YELLOW      0xFFE0
#define TFT_WHITE       0xFFFF
#define TFT_ORANGE      0xFDA0
#define TFT_GREENYELLOW 0xAFE5
#define TFT_PINK        0xF81F

// === Basic compatibility class ===
// It's called TFT_eSPI so that old code will work without modification.
class TFT_eSPI : public lgfx::LGFX_Device
{
    // Driver instances from LovyanGFX
    lgfx::Panel_ST7789  _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;
    lgfx::Touch_XPT2046 _touch_instance; // <-- ADDED TOUCHSCREEN INSTANCE

public:
    // Constructor - all customization for specific hardware occurs here
    TFT_eSPI()
    {
        // 1. SPI bus configuration for the DISPLAY (HSPI)
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host     = HSPI_HOST;
            cfg.spi_mode     = 0;
            cfg.freq_write   = 80000000;      // This board supports 80MHz
            cfg.freq_read    = 16000000;
            cfg.pin_sclk     = 14;            // SCK
            cfg.pin_mosi     = 13;            // MOSI
            cfg.pin_miso     = 12;            // MISO
            cfg.pin_dc       = 2;             // D/C
            cfg.dma_channel  = SPI_DMA_CH_AUTO;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        // 2. ST7789 Panel Configuration
        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs           = 15;        // CS
            cfg.pin_rst          = -1;
            cfg.pin_busy         = -1;
            cfg.panel_width      = 240;
            cfg.panel_height     = 320;
            cfg.offset_x         = 0;
            cfg.offset_y         = 0;
            cfg.offset_rotation  = 0;
            cfg.dummy_read_pixel = 16;
            cfg.readable         = true;
            cfg.invert           = true;      // Correct for this board
            cfg.rgb_order        = false;
            cfg.dlen_16bit       = false;
            cfg.bus_shared       = false;     // The buses for display and touch are separate
            _panel_instance.config(cfg);
        }

        // 3. Backlight configuration
        {
            auto cfg = _light_instance.config();
            cfg.pin_bl       = 21;            // Backlight pin
            cfg.invert       = false;
            cfg.freq         = 12000;
            cfg.pwm_channel  = 7;
            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        // 4. Touchscreen (XPT2046) configuration - ADDED
        {
            auto cfg = _touch_instance.config();
            cfg.spi_host    = VSPI_HOST;      // Uses the OTHER SPI bus!
            cfg.freq        = 1000000;
            cfg.pin_sclk    = 25;
            cfg.pin_mosi    = 32;
            cfg.pin_miso    = 39;
            cfg.pin_cs      = 33;
            cfg.pin_int     = 36;
            cfg.bus_shared  = false;          // It has its own bus
            
            // Calibration values for touch inversion
            cfg.x_min = 3800;
            cfg.x_max = 240;
            cfg.y_min = 200;
            cfg.y_max = 3700;
            
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }

        // Putting it all together
        setPanel(&_panel_instance);
    }

    // === Redirection of popular TFT_eSPI methods ===
    void init()                  { LGFX_Device::init(); setBrightness(255); }
    void setRotation(uint8_t r)  { LGFX_Device::setRotation(r % 8); }
    void fillScreen(uint16_t color) { LGFX_Device::fillScreen(color); }

    void drawPixel(int32_t x, int32_t y, uint16_t color)            { LGFX_Device::drawPixel(x, y, color); }
    void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color) { LGFX_Device::drawLine(x0, y0, x1, y1, color); }
    void drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color) { LGFX_Device::drawRect(x, y, w, h, color); }
    void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color) { LGFX_Device::fillRect(x, y, w, h, color); }
    void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint16_t color) { LGFX_Device::drawRoundRect(x, y, w, h, r, color); }
    void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint16_t color) { LGFX_Device::fillRoundRect(x, y, w, h, r, color); }
    void drawCircle(int32_t x, int32_t y, int32_t r, uint16_t color) { LGFX_Device::drawCircle(x, y, r, color); }
    void fillCircle(int32_t x, int32_t y, int32_t r, uint16_t color) { LGFX_Device::fillCircle(x, y, r, color); }

    // Text
    void setTextColor(uint16_t c)                 { LGFX_Device::setTextColor(c); }
    void setTextColor(uint16_t c, uint16_t bg)    { LGFX_Device::setTextColor(c, bg); }
    void setTextSize(uint8_t s)                   { LGFX_Device::setTextSize(s); }
    void setCursor(int32_t x, int32_t y)          { LGFX_Device::setCursor(x, y); }

    // Print (use inheritance for all overloads)
    using LGFX_Device::print;
    using LGFX_Device::println;
    using LGFX_Device::printf;

    // Touchscreen compatibility - ADDED
    bool getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600) {
        return LGFX_Device::getTouch((int32_t*)x, (int32_t*)y);
    }

    // Additionally
    void setBrightness(uint8_t brightness) { LGFX_Device::setBrightness(brightness); }
    int16_t width()  const { return LGFX_Device::width(); }
    int16_t height() const { return LGFX_Device::height(); }
};
