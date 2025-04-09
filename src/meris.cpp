
#include <Arduino.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <MIDI.h>

#include "button.h"
#include "config.h"
#include "debug.h"
#include "version.h"

#if defined(USE_OLED)
#  include "display/display_oled128x32.h"
#elif defined(USE_LCD)
#  include "display/display_lcd16x2.h"
#endif


int                 _cc_index = 0;
bool                _cc_config = false;
unsigned long       _bank_ts;
int                 _bank = 0;  // goes from 0 to (MAX_BANKS - 1)
int                 _patch = 0; 
SoftwareSerial      _midi_serial(PIN_MIDI_RX, PIN_MIDI_TX);
MIDI_CREATE_INSTANCE(SoftwareSerial, _midi_serial, _midi_out);
IDisplay *          _display = nullptr;

void on_button_event(uint8_t id, EButtonScanResult event);
Button          _button_1(PIN_BUTTON_1, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_2(PIN_BUTTON_2, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_3(PIN_BUTTON_3, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_4(PIN_BUTTON_4, LONGPRESS_DELAY_MS, on_button_event);


void refresh_exp_value() {
    int raw = analogRead(PIN_EXPRESSION);
    meris_cc_t cc_data = MERIS_CC_DATA[_cc_index];
    int exp = map(raw, 0, 1023, cc_data.min, cc_data.max);
    int percent = map(exp, cc_data.min, cc_data.max, 0, 100);
    _display->update_cc_ui(exp, percent);
}


void send_pc(uint8_t command) {
    uint8_t pc = (command & 0x7F);
    _midi_out.sendProgramChange(pc, MIDI_CHANNEL);
}


void send_cc(uint8_t value) {
    uint8_t cc = (value & 0x7F);
    _midi_out.sendControlChange(MERIS_CC_DATA[_cc_index].idx, cc, MIDI_CHANNEL);
}


void on_button_event(uint8_t id, EButtonScanResult event) {

    static bool _cc1_ulp = false;
    static bool _cc2_ulp = false;

    uint8_t idx = MERIS_CC_DATA[_cc_index].idx;
    const __FlashStringHelper * name = MERIS_CC_DATA[_cc_index].name;

    switch (event) {
        case EButtonDown: {
        } break;

        case EButtonUp: {
        } break;

        case EButtonClick: {
            if(_cc_config) {
                // CC config mode: buttons 1 & 4 navigates thru CC values
                if (id == BUTTON_BANK_DOWN) {
                    if (--_cc_index < 0) {
                        _cc_index = MERIS_CC_DATA_LEN - 1;
                    }
                    _display->update_cc_info(idx, name);
                    EEPROM.update(EEPROM_ADDR_CCIDX, _cc_index);
                }
                else if (id == BUTTON_BANK_UP) {
                    if (++_cc_index == MERIS_CC_DATA_LEN) {
                        _cc_index = 0;
                    }
                    _display->update_cc_info(idx, name);
                    EEPROM.update(EEPROM_ADDR_CCIDX, _cc_index);
                }
            }
            else {
                // PC mode: buttons change current patch
                uint8_t pc = (id - PIN_BUTTON_1) + ((_bank * MAX_BUTTONS) + 1);
                _patch = pc;
                send_pc(pc);
                _display->update_bank_ui(_bank, _patch);
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

            if (id == BUTTON_CONFIG_CC_1) {
                _cc1_ulp = true;
            }
            if (id == BUTTON_CONFIG_CC_2) {
                _cc2_ulp = true;
            }
            // CC config mode
            if (_cc1_ulp && _cc2_ulp) {
                _cc_config = !_cc_config;
                dprint(F("CC CONFIG "));
                dprintln(_cc_config ? F("ON") : F("OFF"));
                if (_cc_config) {
                    _display->update_cc_info(idx, name);
                }
                else {
                    _display->update_bank_ui(_bank, _patch);
                    refresh_exp_value();
                }
            }
        } break;
        case EButtonHold: {
            // bank up/down
            if (!_cc_config && millis() - _bank_ts > BANK_INTERVAL_MS) {
                if (id == BUTTON_BANK_DOWN) {
                    if (--_bank < 0) {
                        _bank = MAX_BANKS - 1;
                    }
                    _display->update_bank_ui(_bank, _patch);
                }
                else if (id == BUTTON_BANK_UP) {
                    if (++_bank == MAX_BANKS) {
                        _bank = 0;
                    }
                    _display->update_bank_ui(_bank, _patch);
                }
                _bank_ts = millis();
            }
        } break;
        case EButtonUnlongpress: {
            if (id == BUTTON_CONFIG_CC_1) {
                _cc1_ulp = false;
            }
            if (id == BUTTON_CONFIG_CC_2) {
                _cc2_ulp = false;
            }
        } break;
    }
}


void setup() {

    _midi_serial.begin(31250);
    _midi_out.begin(MIDI_CHANNEL_OFF);
  
    dprintinit(9600);
    dprintln(F("start"));
    dprintln(MERIS_CC_DATA_LEN);
    _bank = EEPROM.read(EEPROM_ADDR_BANK);
    if (_bank > MAX_BANKS) {
        _bank = 0;
    }
    _patch = EEPROM.read(EEPROM_ADDR_PATCH);
    if (_patch > MERIS_MAX_PATCHES) {
        _patch = 0;
    }
    _cc_index = EEPROM.read(EEPROM_ADDR_CCIDX);
    if (_cc_index > MERIS_CC_DATA_LEN) {
        _cc_index = 0;
    }

    #ifdef USE_OLED
        _display = new OLEDDisplay(MERIS_DEVICE_NAME);
    #elif defined(USE_LCD)
        _display = new LCD16x2Display(MERIS_DEVICE_NAME);
    #endif
    delay(2000);
    _display->clear();

    _display->update_bank_ui(_bank, _patch);
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
        meris_cc_t cc_data = MERIS_CC_DATA[_cc_index];
        int exp = map(raw, 0, 1023, cc_data.min, cc_data.max);
        if (exp != _exp_prev) {
            send_cc(exp);
            int percent = map(exp, cc_data.min, cc_data.max, 0, 100);
            _display->update_cc_ui(exp, percent);
            _exp_prev = exp;
        }
    }
}