# 60 Knobs Firmware

Completely written in Arduino  
Depends on [MIDI Library](https://playground.arduino.cc/Main/MIDILibrary) and [EEPROM Library](https://www.arduino.cc/en/Reference/EEPROM)

CC-BY-SA Bastl-Instruments

Updates v1.1.3

* Supports new "CC with Range" and "CC with Range (Specific Channel)" options.
* Flashes the led the number of times corresponding to the menu option:
  ** MIDI channel
  ** preset number
* Added a firmware version check, hold button and turn knob 60 and the led flashes the version number, MAJOR, MINOR, FIX with a pause between.
