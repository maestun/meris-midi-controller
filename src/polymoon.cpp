// #include <MIDI.h>
#include "debug.h"
#include "button.h"

#define MAX_BUTTONS         (4)
#define MAX_PATCHES         (16)
#define MAX_BANKS           (MAX_PATCHES / MAX_BUTTONS)

#define PIN_BUTTON_1        (2)
#define PIN_BUTTON_2        (3)
#define PIN_BUTTON_3        (4)
#define PIN_BUTTON_4        (5)

#define LONGPRESS_DELAY_MS  (500)
#define BANK_INTERVAL_MS    (500)



unsigned long   _bank_ts;
int _bank = 0;
void on_button_event(uint8_t id, EButtonScanResult event);

// MIDI_CREATE_DEFAULT_INSTANCE();

Button          _button_1(PIN_BUTTON_1, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_2(PIN_BUTTON_2, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_3(PIN_BUTTON_3, LONGPRESS_DELAY_MS, on_button_event);
Button          _button_4(PIN_BUTTON_4, LONGPRESS_DELAY_MS, on_button_event);


void update_bank_ui() {
    dprint(F("BANK: "));
    dprintln(_bank + 1);
}

void update_pc_ui(uint8_t pc) {
    dprint(F("PC: "));
    dprintln(pc);
}

void send_pc(uint8_t command) {
    uint8_t pc = (command & 0x7F); // Ensure valid MIDI range (0-127)
    // MIDI.sendProgramChange(pc, 1); // Send on MIDI channel 1
}

void on_button_event(uint8_t id, EButtonScanResult event) {
    switch (event) {
        case EButtonDown: {
        } break;

        case EButtonUp: {
        } break;

        case EButtonClick: {
            uint8_t pc = (id - PIN_BUTTON_1) + ((_bank * MAX_BUTTONS) + 1);
            send_pc(pc);
            update_pc_ui(pc);
        } break;

        case EButtonLongpress: {
            _bank_ts = 0;
        } break;
        case EButtonHold: {
            if (millis() - _bank_ts > BANK_INTERVAL_MS) {
                if (id == PIN_BUTTON_1) {
                    if (++_bank == MAX_BANKS) {
                        _bank = 0;
                    }
                    update_bank_ui();
                }
                else if (id == PIN_BUTTON_4) {
                    if (--_bank < 0) {
                        _bank = MAX_BANKS - 1;
                    }
                    update_bank_ui();
                }
                _bank_ts = millis();
            }
        } break;
        case EButtonUnlongpress: {
        } break;
    }
}



void setup() {
    dprintinit(9600);
    dprintln(F("start"));
    // MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
    _bank = 0;
    update_bank_ui();
}


void loop() {
    _button_1.scan();
    _button_2.scan();
    _button_3.scan();
    _button_4.scan();
}