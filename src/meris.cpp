
#include <Arduino.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <SmoothProgress.h>
#include <BarStyle0.h>
#include <MIDI.h>

#include "button.h"
#include "debug.h"
#include "version.h"

#ifdef USE_OLED
#  include "display_oled.h"
#elif defined(USE_LCD)
#  include "display_lcd16x2.h"
#endif


#if defined(DEVICE_POLYMOON)
#include "polymoon_cc.h" 
#elif defined(DEVICE_MERCURY7)
#include "mercury7_cc.h"
#elif defined(DEVICE_ENZO)
#include "enzo_cc.h"
#elif defined(DEVICE_HEDRA)
#include "hedra_cc.h"
#elif defined(DEVICE_OTTOBIT)
#include "ottobit_cc.h"
#endif

#define MAX_BUTTONS         (4)
#define MAX_PATCHES         (16)
#define MAX_BANKS           (MAX_PATCHES / MAX_BUTTONS)

#define PIN_BUTTON_1        (2)
#define PIN_BUTTON_2        (3)
#define PIN_BUTTON_3        (4)
#define PIN_BUTTON_4        (5)
#define PIN_EXPRESSION      (A0)
#define PIN_MIDI_TX         (10)
#define PIN_MIDI_RX         (255) // disabled

#define LONGPRESS_DELAY_MS  (500)
#define BANK_INTERVAL_MS    (500)

#define EEPROM_ADDR_BANK    (0)
#define EEPROM_ADDR_PATCH   (1)
#define EEPROM_ADDR_CCIDX   (2)

int                 _cc_index = 0;
bool                _cc_config = false;
unsigned long       _bank_ts;
int                 _bank = 0;  // goes from 0 to (MAX_BANKS - 1)
int                 _patch = 0; 
LiquidCrystal_I2C   _display(0x27, 16, 2);
LCD                 _pg_lcd(_display, barStyle0);
SmoothProgressBar   _progress(_pg_lcd, 16, 0, 1, 1);
SoftwareSerial      _midi_serial(PIN_MIDI_RX, PIN_MIDI_TX);
MIDI_CREATE_INSTANCE(SoftwareSerial, _midi_serial, _midi_out);


void on_button_event(uint8_t id, EButtonScanResult event);
Button          _button_1(PIN_BUTTON_1, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_2(PIN_BUTTON_2, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_3(PIN_BUTTON_3, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_4(PIN_BUTTON_4, LONGPRESS_DELAY_MS, on_button_event);


void update_bank_ui(int bank, int patch) {
    dprint(F("BANK: "));
    dprintln(bank);
    _display.setCursor(0, 0);
    _display.print(F("      "));
    _display.setCursor(0, 0);
    _display.print(F("B"));
    _display.print(bank + 1);
    _display.print(F(" P"));
    _display.print(patch);
    _display.display();
}


void update_cc_ui(uint8_t cc_val, uint8_t percent) {
    dprint(F("CC VAL: "));
    dprintln(cc_val);
    dprint(F(" - "));
    dprintln(percent);

    _display.setCursor(9, 0);
    _display.print(F("       "));
    _display.setCursor(9, 0);
    _display.print(F("EXP "));
    _display.print(cc_val);
    _progress.showProgressPct(percent);
    _display.display();
}


void update_cc_info() {
    uint8_t idx = CC_DATA[_cc_index].idx;
    const __FlashStringHelper * name = CC_DATA[_cc_index].name;
    dprint(F("CC #"));
    dprint(idx);
    dprint(F(" - "));
    dprintln(name);
    dprint(F(" - "));
    dprintln(name);

    _display.clear();
    _display.setCursor(0, 0);
    _display.print(F("CC #"));
    _display.print(idx);
    _display.setCursor(0, 1);
    _display.print(name);
    _display.display();
}


void refresh_exp_value() {
    int raw = analogRead(PIN_EXPRESSION);
    meris_cc_t cc_data = CC_DATA[_cc_index];
    int exp = map(raw, 0, 1023, cc_data.min, cc_data.max);
    int percent = map(exp, cc_data.min, cc_data.max, 0, 100);
    update_cc_ui(exp, percent);
}


void send_pc(uint8_t command) {
    uint8_t pc = (command & 0x7F); // Ensure valid MIDI range (0-127)
    // MIDI.sendProgramChange(pc, 1); // Send on MIDI channel 1
    _midi_out.sendProgramChange(pc, MIDI_CHANNEL);
}


void send_cc(uint8_t value) {
    uint8_t cc = (value & 0x7F); // Ensure valid MIDI range (0-127)
    // MIDI.sendControlChange(CC_DATA[_cc_index].idx, cc, 1);
    _midi_out.sendControlChange(CC_DATA[_cc_index].idx, cc, MIDI_CHANNEL);
}


void on_button_event(uint8_t id, EButtonScanResult event) {

    static bool _bt2_ulp = false;
    static bool _bt3_ulp = false;

    switch (event) {
        case EButtonDown: {
        } break;

        case EButtonUp: {
        } break;

        case EButtonClick: {
            if(_cc_config) {
                // CC config mode: buttons 1 & 4 navigates thru CC values
                if (id == PIN_BUTTON_1) {
                    if (--_cc_index < 0) {
                        _cc_index = CC_DATA_LEN - 1;
                    }
                    update_cc_info();
                    EEPROM.update(EEPROM_ADDR_CCIDX, _cc_index);
                }
                else if (id == PIN_BUTTON_4) {
                    if (++_cc_index == CC_DATA_LEN) {
                        _cc_index = 0;
                    }
                    update_cc_info();
                    EEPROM.update(EEPROM_ADDR_CCIDX, _cc_index);
                }
            }
            else {
                // PC mode: buttons change current patch
                uint8_t pc = (id - PIN_BUTTON_1) + ((_bank * MAX_BUTTONS) + 1);
                _patch = pc;
                send_pc(pc);
                update_bank_ui(_bank, _patch);
                EEPROM.update(EEPROM_ADDR_BANK, _bank & 0xff);
                EEPROM.update(EEPROM_ADDR_PATCH, _patch & 0xff);
                dprint(F("SAVED "));
                dprint(_bank);
                dprint(F("-"));
                dprintln(_patch);
            }
        } break;

        case EButtonLongpress: {
            _bank_ts = 0;

            if (id == PIN_BUTTON_2) {
                _bt2_ulp = true;
            }
            if (id == PIN_BUTTON_3) {
                _bt3_ulp = true;
            }
            // CC config mode
            if (_bt2_ulp && _bt3_ulp) {
                _cc_config = !_cc_config;
                dprint(F("CC CONFIG "));
                dprintln(_cc_config ? F("ON") : F("OFF"));
                if (_cc_config) {
                    update_cc_info();
                }
                else {
                    update_bank_ui(_bank, _patch);
                    refresh_exp_value();
                }
            }
        } break;
        case EButtonHold: {
            // bank up/down
            if (!_cc_config && millis() - _bank_ts > BANK_INTERVAL_MS) {
                if (id == PIN_BUTTON_1) {
                    if (--_bank < 0) {
                        _bank = MAX_BANKS - 1;
                    }
                    update_bank_ui(_bank, _patch);
                    // EEPROM.put(EEPROM_ADDR_BANK, _bank & 0xff);
                }
                else if (id == PIN_BUTTON_4) {
                    if (++_bank == MAX_BANKS) {
                        _bank = 0;
                    }
                    update_bank_ui(_bank, _patch);
                    // EEPROM.put(EEPROM_ADDR_BANK, _bank & 0xff);
                }
                _bank_ts = millis();
            }
        } break;
        case EButtonUnlongpress: {
            if (id ==  PIN_BUTTON_2) {
                _bt2_ulp = false;
            }
            if (id ==  PIN_BUTTON_3) {
                _bt3_ulp = false;
            }
        } break;
    }
}


void setup() {

    _midi_serial.begin(31250);
    // _midi_out.begin();
    _midi_out.begin(MIDI_CHANNEL_OFF);
  
    dprintinit(9600);
    dprintln(F("start"));
    dprintln(CC_DATA_LEN);
    // MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
    _bank = EEPROM.read(EEPROM_ADDR_BANK);
    if (_bank > MAX_BANKS) {
        _bank = 0;
    }
    _patch = EEPROM.read(EEPROM_ADDR_PATCH);
    if (_patch > MAX_PATCHES) {
        _patch = 0;
    }
    _cc_index = EEPROM.read(EEPROM_ADDR_CCIDX);
    if (_cc_index > CC_DATA_LEN) {
        _cc_index = 0;
    }

    _display.init();
    _display.clear();
    _display.backlight();
    _pg_lcd.begin();

    _display.setCursor(0, 0);
    _display.print(MERIS_DEVICE_NAME);
    _display.setCursor(0, 1);
    _display.print(GIT_HASH);
    _display.setCursor(11, 1);
    _display.print(GIT_TAG);
    delay(2000);
    _display.clear();

    update_bank_ui(_bank, _patch);
    refresh_exp_value();
}


void loop() {
    static int _exp_prev = 0;
    _button_1.scan();
    _button_2.scan();
    _button_3.scan();
    _button_4.scan();

    if (!_cc_config) {
        int raw = analogRead(PIN_EXPRESSION);
        // TODO: filter ?
        // ...
        meris_cc_t cc_data = CC_DATA[_cc_index];
        int exp = map(raw, 0, 1023, cc_data.min, cc_data.max);
        if (exp != _exp_prev) {
            send_cc(exp);
            int percent = map(exp, cc_data.min, cc_data.max, 0, 100);
            update_cc_ui(exp, percent);
            _exp_prev = exp;
        }
    }
}