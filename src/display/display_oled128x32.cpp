#include "display_oled128x32.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "debug.h"
#include "version.h"

#define PIN_OLED_RESET              (-1)
#define SCREEN_WIDTH                (128)
#define SCREEN_HEIGHT               (32)

#define CHAR_HEIGHT                 (16)
#define CHAR_WIDTH                  (12)

// display stuff
Adafruit_SSD1306 	_oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, PIN_OLED_RESET);

OLEDDisplay::OLEDDisplay(const String device_name) {
    if(!_oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        // TODO: show error
    }
    clear();
    _oled.setTextSize(2);
    _oled.setTextColor(SSD1306_WHITE);   
    _oled.setCursor(0, 0);
    _oled.print(device_name);
    _oled.setCursor(0, 1 * CHAR_HEIGHT);
    _oled.print(GIT_TAG);
    _oled.display();
}

void OLEDDisplay::clear() {
    _oled.clearDisplay();
}

void OLEDDisplay::update_bank_ui(int bank, int patch) {
    dprint(F("BANK: "));
    dprintln(bank);
    clear();
    _oled.setCursor(0, 0);
    _oled.print(F("      "));
    _oled.display();
    _oled.setCursor(0, 0);
    _oled.print(F("B"));
    _oled.print(bank + 1);
    _oled.print(F(" P"));
    _oled.print(patch);
    _oled.display();
}


void OLEDDisplay::update_cc_value(uint8_t cc_val, uint8_t percent) {
    // _oled.setCursor(0, 1 * CHAR_HEIGHT);
    // _oled.print(F("        "));
    // _oled.display();
    // _oled.setCursor(0, 1 * CHAR_HEIGHT);
    // _oled.print(F("EXP "));
    // _oled.print(cc_val);
    // _oled.display();
    _oled.fillRect(0, 1 * CHAR_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT / 2, SSD1306_BLACK);
    _oled.fillRect(0, 1 * CHAR_HEIGHT, map(percent, 0, 100, 0, SCREEN_WIDTH), SCREEN_HEIGHT / 2, SSD1306_WHITE);
    _oled.display();
}


void OLEDDisplay::update_cc_name(uint8_t cc) {
    clear();
    _oled.setCursor(0, 0);
    _oled.print(cc);
    _oled.display();
}

void OLEDDisplay::update_cc_name(const __FlashStringHelper * name) {
    clear();
    _oled.setCursor(0, 0);
    _oled.print(name);
    _oled.display();
}