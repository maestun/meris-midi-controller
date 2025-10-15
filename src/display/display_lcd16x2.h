#pragma once

#include "display.h"

class LCD16x2Display : public IDisplay {

public: 
    LCD16x2Display(const String device_name);
    void clear() override;
    void update_bank_ui(int bank, int patch);
    void update_cc_value(uint8_t cc_val, uint8_t percent);
    void update_cc_name(uint8_t idx);
    void update_cc_name(const __FlashStringHelper * name);
};