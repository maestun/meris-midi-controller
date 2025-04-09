#pragma once

#include "meris.h"

// ui: number of buttons on the pedal
#define MAX_BUTTONS         (4)
#define MAX_BANKS           (MERIS_MAX_PATCHES / MAX_BUTTONS)

// button wiring: PIN --> button --> GND
#define PIN_BUTTON_1        (2)
#define PIN_BUTTON_2        (3)
#define PIN_BUTTON_3        (4)
#define PIN_BUTTON_4        (5)

// expression pedal wiring: pot1 --> GND / pot2 --> PIN / pot3 --> VCC
#define PIN_EXPRESSION      (A0)

// MIDI TRS wiring: TX PIN --> jack tip / jack sleeve --> GND (leave ring floating)
#define PIN_MIDI_TX         (10)
#define PIN_MIDI_RX         (255) // disabled

// delay before longpress is detected
#define LONGPRESS_DELAY_MS  (500)

// interval between two banks scrolling
#define BANK_INTERVAL_MS    (500)

// eeprom addresses
#define EEPROM_ADDR_BANK    (0)
#define EEPROM_ADDR_PATCH   (1)
#define EEPROM_ADDR_CCIDX   (2)

// ui config
#define BUTTON_BANK_UP      (PIN_BUTTON_4)
#define BUTTON_BANK_DOWN    (PIN_BUTTON_1)
#define BUTTON_CONFIG_CC_1  (PIN_BUTTON_2)
#define BUTTON_CONFIG_CC_2  (PIN_BUTTON_3)