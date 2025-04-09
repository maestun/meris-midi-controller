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
        {   16, 0,  127,    F("SPACE DECAY") },
        {   17, 0,  127,    F("MODULATE") },
        {   18, 0,  127,    F("MIX") },
        {   19, 0,  127,    F("LO FREQ") },
        {   20, 0,  127,    F("PITCH VECTOR") },
        {   21, 0,  127,    F("HI FREQ") },
        {   22, 0,  127,    F("PREDELAY") },
        {   23, 0,  127,    F("MOD SPEED") },
        {   24, 0,  127,    F("PITCH VECTOR MIX") },
        {   25, 0,  127,    F("DENSITY") },
        {   26, 0,  127,    F("ATTACK TIME") },
        {   27, 0,  127,    F("VIBRATO DEPTH") },
        {   28, 0,  127,    F("SWELL") },
        {   29, 0,  127,    F("ALGORITHM SELECT") },
    };
    *len = sizeof(polymoon_cc_data) / sizeof(meris_cc_t);
    return polymoon_cc_data;
}

const __FlashStringHelper * build_name () {
    static const __FlashStringHelper * name = F("    MERCURY7    ");
    return name;
}

int CC_DATA_LEN;
static const meris_cc_t* CC_DATA = build_cc_data(&CC_DATA_LEN);
const __FlashStringHelper * MERIS_DEVICE_NAME = build_name();
