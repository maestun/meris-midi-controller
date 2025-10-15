#ifndef IDISPLAY_H
#define IDISPLAY_H

#include <Arduino.h>

class IDisplay {
public:
    virtual ~IDisplay() = default; // No need for "override" here!
    virtual void clear() = 0;
    virtual void update_bank_ui(int bank, int patch) = 0;
    virtual void update_cc_value(uint8_t cc_val, uint8_t percent) = 0;
    virtual void update_cc_name(uint8_t cc) = 0;
    virtual void update_cc_name(const __FlashStringHelper * name) = 0;
};

#endif
