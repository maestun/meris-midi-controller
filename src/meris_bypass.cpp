
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


int             _cc_index = 0; // index of current select CC
bool            _cc_config = false;
unsigned long   _bank_ts;
int             _bank = 0;  // goes from 0 to (MAX_BANKS - 1)
int             _patch = 0; 
SoftwareSerial  _midi_serial(PIN_MIDI_RX, PIN_MIDI_TX);
MIDI_CREATE_INSTANCE(SoftwareSerial, _midi_serial, _midi_out);
IDisplay *      _display = nullptr;

void on_button_event(uint8_t id, EButtonScanResult event);

typedef struct {
    bool fx_on;
    int led_pin;
    Button button;
} button_t;

button_t _buttons[] = {
    { false, PIN_LED_1, Button(0, PIN_BUTTON_1, LONGPRESS_DELAY_MS, on_button_event) },
    { false, PIN_LED_2, Button(1, PIN_BUTTON_2, LONGPRESS_DELAY_MS, on_button_event) },
    { false, PIN_LED_3, Button(2, PIN_BUTTON_3, LONGPRESS_DELAY_MS, on_button_event) },
    { false, PIN_LED_4, Button(3, PIN_BUTTON_4, LONGPRESS_DELAY_MS, on_button_event) },
};

void refresh_exp_value() {
    meris_cc_t cc_data = MERIS_CC_DATA[_cc_index];
    if (cc_data.cc != MERIS_CC_NONE) {
        digitalWrite(PIN_EXPRESSION_LED, HIGH);
        int raw = analogRead(PIN_EXPRESSION);
        int exp = map(raw, 0, 1023, cc_data.min, cc_data.max);
        int percent = map(exp, cc_data.min, cc_data.max, 0, 100);
        _display->update_cc_value(exp, percent);
    }
    else {
        digitalWrite(PIN_EXPRESSION_LED, LOW);
    }
}

void send_pc(uint8_t command) {
    uint8_t pc = (command & 0x7F);
    _midi_out.sendProgramChange(pc, MIDI_CHANNEL);
}

void send_cc(uint8_t value, int cc_index = _cc_index) {
    uint8_t val = (value & 0x7F);
    _midi_out.sendControlChange(MERIS_CC_DATA[cc_index].cc, val, MIDI_CHANNEL);
}

void update_cc() {
    uint8_t cc = MERIS_CC_DATA[_cc_index].cc;
    const __FlashStringHelper * name = MERIS_CC_DATA[_cc_index].name;
    _display->update_cc_name(cc);
    _display->update_cc_name(name);
    EEPROM.update(EEPROM_ADDR_CCIDX, _cc_index);
}

void on_button_event(uint8_t id, EButtonScanResult event) {

    switch (event) {
        case EButtonDown: {
        } break;

        case EButtonUp: {
        } break;

        case EButtonClick: {
            // PC mode: buttons change current patch or bypasses
            button_t& button = _buttons[id];
            if (button.fx_on) {
                // FX OFF
                digitalWrite(button.led_pin, LOW);
                send_cc(0, MERIS_CC_IDX_BYPASS);
                button.fx_on = false;

            }
            else {
                // send patch + FX ON
                uint8_t pc = (id - PIN_BUTTON_1) + ((_bank * MAX_BUTTONS) + 1);
                _patch = pc;
                send_pc(pc);                        // change patch
                send_cc(127, MERIS_CC_IDX_BYPASS);  // FX ON TODO: maybe not needed
                digitalWrite(button.led_pin, HIGH);
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
            // if (id == BUTTON_CONFIG_CC_1) {
            //     _cc1_ulp = true;
            // }
            // if (id == BUTTON_CONFIG_CC_2) {
            //     _cc2_ulp = true;
            // }
            // // CC config mode
            // if (_cc1_ulp && _cc2_ulp) {
            //     _cc_config = !_cc_config;
            //     dprint(F("CC CONFIG "));
            //     dprintln(_cc_config ? F("ON") : F("OFF"));
            //     if (_cc_config) {

            //         // dprint(F("IDX "));
            //         // dprintln(_cc_index);
            //         update_cc();
            //         // _display->update_cc_name(cc, name);
            //     }
            //     else {
            //         _display->update_bank_ui(_bank, _patch);
            //         refresh_exp_value();
            //     }
            // }
        } break;
        case EButtonHold: {
            // bank up/down
            if (/*!_cc_config &&*/
                millis() - _bank_ts > BANK_INTERVAL_MS) {
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
            // if (id == BUTTON_CONFIG_CC_1) {
            //     _cc1_ulp = false;
            // }
            // if (id == BUTTON_CONFIG_CC_2) {
            //     _cc2_ulp = false;
            // }
        } break;
    }
}


void setup() {
    pinMode(PIN_EXPRESSION_LED, OUTPUT);

    _midi_serial.begin(31250);
    _midi_out.begin(MIDI_CHANNEL_OFF);
  
    dprintinit(9600);
    dprintln(F("start"));
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

    for (auto bt : _buttons) {
        pinMode(bt.led_pin, OUTPUT);
    }

    #ifdef USE_OLED
        _display = new OLEDDisplay(MERIS_DEVICE_NAME);
    #elif defined(USE_LCD)
        _display = new LCD16x2Display(MERIS_DEVICE_NAME);
    #endif
    delay(2000);
    _display->clear();

    _display->update_bank_ui(_bank, _patch);
    const __FlashStringHelper * name = MERIS_CC_DATA[_cc_index].name;
    _display->update_cc_name(name);
    refresh_exp_value();
}


void loop() {

    static const int THRESHOLD = 1;

    static int _exp_prev = 0;
    for (auto bt : _buttons) {
        bt.button.scan();
    }

    if (/*!_cc_config*/true) {
        meris_cc_t cc_data = MERIS_CC_DATA[_cc_index];
        if (cc_data.cc != MERIS_CC_NONE) {
            digitalWrite(PIN_EXPRESSION_LED, HIGH);

            // TODO: smarter filter ?
            int raw = analogRead(PIN_EXPRESSION);
            int exp = map(raw, 0, 1023, cc_data.min, cc_data.max);
            if (exp < _exp_prev - THRESHOLD || exp > _exp_prev + THRESHOLD) {
                send_cc(exp);
                int percent = map(exp, cc_data.min, cc_data.max, 0, 100);
                _display->update_cc_value(exp, percent);
                _exp_prev = exp;
            }
        }
        else {
            digitalWrite(PIN_EXPRESSION_LED, LOW);
        }
    }
}