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
        {    9, 0,  127,    F("DOTTED 8TH") },
        {   14, 0,  127,    F("BYPASS") },
        {   15, 0,  120,    F("TEMPO") },
        {   16, 0,  127,    F("TIME") },
        {   17, 0,  127,    F("FEEDBACK") },
        {   18, 0,  127,    F("MIX") },
        {   19, 0,  127,    F("MULTIPLY") },
        {   20, 0,  127,    F("DIMENSION") },
        {   21, 0,  127,    F("DYNAMICS") },
        {   22, 0,  127,    F("EARLY MODULATION") },
        {   23, 0,  127,    F("FEEDBACK FILTER") },
        {   24, 0,  127,    F("DELAY LEVEL") },
        {   25, 0,  127,    F("LATE MODULATION") },
        {   26, 0,  127,    F("DYN FLANGE MODE") },
        {   27, 0,  127,    F("DYN FLANGE SPEED") },
        {   28, 0,  127,    F("TAP") },
        {   29, 0,  127,    F("PHASER MODE") },
        {   30, 0,  127,    F("FLANGER FEEDBACK") },
        {   31, 0,  127,    F("HALF SPEED") },
    };
    *len = sizeof(polymoon_cc_data) / sizeof(meris_cc_t);
    return polymoon_cc_data;
}

int CC_DATA_LEN;
static const meris_cc_t* CC_DATA = build_cc_data(&CC_DATA_LEN);

