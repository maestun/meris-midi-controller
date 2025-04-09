#include <Arduino.h>

typedef struct {
    uint8_t idx;
    uint8_t min;
    uint8_t max;
    const __FlashStringHelper * name;
} meris_cc_t;

meris_cc_t* build_cc_data(int* len) {
    static meris_cc_t polymoon_cc_data[] = {
        {    4, 0,  127,    F("EXP PEDAL") },
        {    9, 0,  127,    F("ENVELOPE TYPE") },
        {   14, 0,  127,    F("BYPASS") },
        {   15, 0,  120,    F("TEMPO") },
        {   16, 0,  127,    F("PITCH") },
        {   17, 0,  127,    F("FILTER") },
        {   18, 0,  127,    F("MIX") },
        {   19, 0,  127,    F("SUSTAIN") },
        {   20, 0,  127,    F("FILTER ENVELOPE") },
        {   21, 0,  127,    F("MODULATION") },
        {   22, 0,  127,    F("PORTAMENTO") },
        {   23, 0,  127,    F("FILTER TYPE") },
        {   24, 0,  127,    F("DELAY LEVEL") },
        {   25, 0,  127,    F("RING MODULATION") },
        {   26, 0,  127,    F("FILTER BANDWIDTH") },
        {   27, 0,  127,    F("DELAY FEEDBACK") },
        {   28, 0,  127,    F("TAP") },
        {   29, 0,  127,    F("SYNTH MODE") },
        {   30, 0,  127,    F("SYNTH WAVESHAPE") },
    };
    *len = sizeof(polymoon_cc_data) / sizeof(meris_cc_t);
    return polymoon_cc_data;
}

const __FlashStringHelper * build_name () {
    static const __FlashStringHelper * name = F("      ENZO      ");
    return name;
}

int CC_DATA_LEN;
static const meris_cc_t* CC_DATA = build_cc_data(&CC_DATA_LEN);
const __FlashStringHelper * MERIS_DEVICE_NAME = build_name();
