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
        {   14, 0,  127,    F("BYPASS") },
        {   15, 0,  127,    F("TEMPO") },
        {   16, 0,  127,    F("SAMPLE RATE") },
        {   17, 0,  127,    F("FILTER") },
        {   18, 0,  127,    F("BITS") },
        {   19, 0,  127,    F("STUTTER") },
        {   20, 0,  127,    F("SEQUENCER") },
        {   21, 0,  127,    F("SEQUENCER MULT") },
        {   22, 0,  127,    F("STEP 1") },
        {   23, 0,  127,    F("STEP 2") },
        {   24, 0,  127,    F("STEP 3") },
        {   25, 0,  127,    F("STEP 4") },
        {   26, 0,  127,    F("STEP 5") },
        {   27, 0,  127,    F("STEP 6") },
        {   28, 0,  127,    F("TAP") },
        {   29, 0,  127,    F("SEQUENCER TYPE") },
        {   31, 0,  127,    F("STUTTER HOLD") },
    };
    *len = sizeof(polymoon_cc_data) / sizeof(meris_cc_t);
    return polymoon_cc_data;
}

const __FlashStringHelper * build_name () {
    static const __FlashStringHelper * name = F("    OTTOBIT     ");
    return name;
}

int CC_DATA_LEN;
static const meris_cc_t* CC_DATA = build_cc_data(&CC_DATA_LEN);
const __FlashStringHelper * MERIS_DEVICE_NAME = build_name();
