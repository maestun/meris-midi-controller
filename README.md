# Meris MIDI Controller

PlatformIO project, allows full MIDI control of Meris 'small box' pedals:
- preset selection (1 to 16)
- CC messages with an optional expression pedal
- Tested with Polymoon, should also work with Mercury7 / Hedra / Enzo / Ottobit (configure environments in `platform.ini`

## Wiring

- See `src/config.h` file to wire the buttons
- Expression pedal is assigned to `A0` pin, connect a TRS jack: tip to +5v, ring to `A0`, sleeve to ground
- Connect LCD I2C screen's 4 pins: `A4` to `SDA`, `A5` to `SCL`, and +5v / ground. Same for OLED
- Connect MIDI TRS jack to the Meris pedal: tip to a 220 ohm resistor then pin 10, sleeve to ground, leave ring floating

## Usage

- Click on a preset button: activate preset number 1 to 16
- Longpress / hold preset button 1 or 4: switch to bank 1 to 4, each bank contains 4 presets, allowing to navigate thru 16 presets
- Longpress buttons 2 and 3 simultaneously: select CC assigned to expression pedal, you can loop thru CCs using buttons 1 and 4
-  Longpress buttons 2 and 3 simultaneously again to leave CC configuration mode
