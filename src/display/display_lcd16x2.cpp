
#define USE_SERIAL_1602_LCD

#include "display_lcd16x2.h"

#include <LiquidCrystal_I2C.h>
#include <SmoothProgress.h>
#include <BarStyle0.h>

#include "debug.h"
#include "version.h"

#define CHARS_PER_LINE  (16)
#define LINE_COUNT      (16)

LiquidCrystal_I2C   _lcd(0x27, CHARS_PER_LINE, LINE_COUNT);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LCD                 _progress_lcd(_lcd, barStyle0);
SmoothProgressBar   _progress(_progress_lcd, CHARS_PER_LINE, 0, 1, 1);

LCD16x2Display::LCD16x2Display(const String device_name) {
    _lcd.init();
    _lcd.backlight();
    _progress_lcd.begin();
    _lcd.setCursor((CHARS_PER_LINE - device_name.length()) >> 1, 0);
    _lcd.print(device_name);
    _lcd.setCursor(0, 1);
    _lcd.print(GIT_HASH);
    _lcd.setCursor(11, 1);
    _lcd.print(GIT_TAG);
}

void LCD16x2Display::clear() {
    _lcd.clear();
}

void LCD16x2Display::update_bank_ui(int bank, int patch) {
    dprint(F("BANK: "));
    dprintln(bank);
    _lcd.setCursor(0, 0);
    _lcd.print(F("      "));
    _lcd.setCursor(0, 0);
    _lcd.print(F("B"));
    _lcd.print(bank + 1);
    _lcd.print(F(" P"));
    _lcd.print(patch);
    _lcd.display();
}


void LCD16x2Display::update_cc_ui(uint8_t cc_val, uint8_t percent) {
    dprint(F("CC VAL: "));
    dprintln(cc_val);
    dprint(F(" - "));
    dprintln(percent);

    _lcd.setCursor(9, 0);
    _lcd.print(F("        "));
    _lcd.setCursor(9, 0);
    _lcd.print(F("EXP "));
    _lcd.print(cc_val);
    _progress.showProgressPct(percent);
}


void LCD16x2Display::update_cc_info(uint8_t cc, const __FlashStringHelper * name) {
    dprint(F("CC #"));
    dprint(cc);
    dprint(F(" - "));
    dprintln(name);
    dprint(F(" - "));
    dprintln(name);

    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(F("CC #"));
    _lcd.print(cc);
    _lcd.setCursor(0, 1);
    _lcd.print(name);
    _lcd.display();
}
