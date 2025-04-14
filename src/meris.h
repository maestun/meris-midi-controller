#pragma once

#include <Arduino.h>

#define MERIS_MAX_PATCHES   (16)
#define MERIS_CC_NONE       (0)

typedef struct {
    uint8_t cc;
    uint8_t min;
    uint8_t max;
    const __FlashStringHelper * name;
} meris_cc_t;

meris_cc_t* build_cc_data(int* len) {
    static meris_cc_t _cc_data[] = {
#if defined(DEVICE_ENZO)
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
#elif defined(DEVICE_HEDRA)
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
#elif defined(DEVICE_MERCURY7)
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
#elif defined(DEVICE_OTTOBIT)
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
#elif defined(DEVICE_POLYMOON)
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
#endif
        {   MERIS_CC_NONE,  0,  0,      F("EXP OFF") },
    };
    *len = sizeof(_cc_data) / sizeof(meris_cc_t);
    return _cc_data;
}


const String build_name () {
#if defined(DEVICE_ENZO)
    static const String name = F("ENZO");
#elif defined(DEVICE_HEDRA)
    static const String name = F("HEDRA");
#elif defined(DEVICE_MERCURY7)
    static const String name = F("MERCURY7");
#elif defined(DEVICE_OTTOBIT)
    static const String name = F("OTTOBIT");
#elif defined(DEVICE_POLYMOON)
    static const String name = "POLYMOON";
#endif
    return name;
}

int MERIS_CC_DATA_LEN;
static const meris_cc_t* MERIS_CC_DATA = build_cc_data(&MERIS_CC_DATA_LEN);
const String MERIS_DEVICE_NAME = build_name();