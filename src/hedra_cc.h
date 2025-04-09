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
        {    9, 0,  127,    F("HALF SPEED") },
        {   14, 0,  127,    F("BYPASS") },
        {   15, 0,  127,    F("TEMPO") },
        {   16, 0,  127,    F("KEY") },
        {   17, 0,  127,    F("MICRO TUNE") },
        {   18, 0,  127,    F("MIX") },
        {   19, 0,  127,    F("PITCH 1") },
        {   20, 0,  127,    F("PITCH 2") },
        {   21, 0,  127,    F("PITCH 3") },
        {   22, 0,  127,    F("SCALE TYPE") },
        {   23, 0,  127,    F("PITCH CORR/GLIDE") },
        {   24, 0,  127,    F("FEEDBACK") },
        {   25, 0,  127,    F("TIME DIVISION 1") },
        {   26, 0,  127,    F("TIME DIVISION 2") },
        {   27, 0,  127,    F("TIME DIVISION 3") },
        {   28, 0,  127,    F("TAP") },
        {   29, 0,  127,    F("DELAY MODE") },
        {   30, 0,  127,    F("PITCH CTL SMOOTH") },
        {   31, 0,  127,    F("VOLUME SWELL") },
    };
    *len = sizeof(polymoon_cc_data) / sizeof(meris_cc_t);
    return polymoon_cc_data;
}

const __FlashStringHelper * build_name () {
    static const __FlashStringHelper * name = F("     HEDRA      ");
    return name;
}

int CC_DATA_LEN;
static const meris_cc_t* CC_DATA = build_cc_data(&CC_DATA_LEN);
const __FlashStringHelper * MERIS_DEVICE_NAME = build_name();
