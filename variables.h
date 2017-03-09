#ifndef _VARIABLES_H_
#define _VARIABLES_H_

const uint8_t knobLUT[60] =
{ 42, 58, 16, 8, 0, 24, 40, 56, 48, 32, 50, 34, 19, 11, 3, 27, 43, 59, 51, 35, 17, 9, 21, 13, 5, 29, 45, 61,
  53, 37, 1, 25, 23, 15, 7, 31, 47, 63, 55, 39, 41, 57, 22, 14, 6, 30, 46, 62, 54, 38, 49, 33, 20, 12, 4, 28, 44, 60, 52, 36
};

const uint8_t factory_preset_1[PRESET_LENGTH] = {
  1,   //MIDI CHANNEL : 1
  /*            First Row                 */
  1, 0x00, 0x00, //Knob 0 : CC, range 0-127
  1, 0x00, 0x00, //Knob 1 : CC, range 0-127
  1, 0x00, 0x00, //Knob 2 : CC, range 0-127
  3, 0x00, 0x00, //Knob 3 : CC, range 0-127
  4, 0x00, 0x00, //Knob 4 : CC, range 0-127
  5, 0x00, 0x00, //Knob 5 : CC, range 0-127
  6, 0x00, 0x00, //Knob 6 : CC, range 0-127
  7, 0x00, 0x00, //Knob 7 : CC, range 0-127
  8, 0x00, 0x00, //Knob 8 : CC, range 0-127
  9, 0x00, 0x00, //Knob 9 : CC, range 0-127

  /*            Second Row                 */
  10, 0x00, 0x00, //Knob 10 : CC, range 0-127
  11, 0x00, 0x00, //Knob 11 : CC, range 0-127
  12, 0x00, 0x00, //Knob 12 : CC, range 0-127
  13, 0x00, 0x00, //Knob 13 : CC, range 0-127
  14, 0x00, 0x00, //Knob 14 : CC, range 0-127
  15, 0x00, 0x00, //Knob 15 : CC, range 0-127
  16, 0x00, 0x00, //Knob 16 : CC, range 0-127
  17, 0x00, 0x00, //Knob 17 : CC, range 0-127
  18, 0x00, 0x00, //Knob 18 : CC, range 0-127
  19, 0x00, 0x00, //Knob 19 : CC, range 0-127

  /*            Third Row                  */
  20, 0x00, 0x00, //Knob 20 : CC, range 0-127
  21, 0x00, 0x00, //Knob 21 : CC, range 0-127
  22, 0x00, 0x00, //Knob 22 : CC, range 0-127
  23, 0x00, 0x00, //Knob 23 : CC, range 0-127
  24, 0x00, 0x00, //Knob 24 : CC, range 0-127
  25, 0x00, 0x00, //Knob 25 : CC, range 0-127
  26, 0x00, 0x00, //Knob 26 : CC, range 0-127
  27, 0x00, 0x00, //Knob 27 : CC, range 0-127
  28, 0x00, 0x00, //Knob 28 : CC, range 0-127
  29, 0x00, 0x00, //Knob 29 : CC, range 0-127

  /*            4th Row                    */
  30, 0x00, 0x00, //Knob 30 : CC, range 0-127
  31, 0x00, 0x00, //Knob 31 : CC, range 0-127
  32, 0x00, 0x00, //Knob 32 : CC, range 0-127
  33, 0x00, 0x00, //Knob 33 : CC, range 0-127
  34, 0x00, 0x00, //Knob 34 : CC, range 0-127
  35, 0x00, 0x00, //Knob 35 : CC, range 0-127
  36, 0x00, 0x00, //Knob 36 : CC, range 0-127
  37, 0x00, 0x00, //Knob 37 : CC, range 0-127
  38, 0x00, 0x00, //Knob 38 : CC, range 0-127
  39, 0x00, 0x00, //Knob 39 : CC, range 0-127

  /*            5th Row                    */
  40, 0x00, 0x00, //Knob 40 : CC, range 0-127
  41, 0x00, 0x00, //Knob 41 : CC, range 0-127
  42, 0x00, 0x00, //Knob 42 : CC, range 0-127
  43, 0x00, 0x00, //Knob 43 : CC, range 0-127
  44, 0x00, 0x00, //Knob 44 : CC, range 0-127
  45, 0x00, 0x00, //Knob 45 : CC, range 0-127
  46, 0x00, 0x00, //Knob 46 : CC, range 0-127
  47, 0x00, 0x00, //Knob 47 : CC, range 0-127
  48, 0x00, 0x00, //Knob 48 : CC, range 0-127
  49, 0x00, 0x00, //Knob 49 : CC, range 0-127

  /*            6th Row                    */
  50, 0x00, 0x00, //Knob 50 : CC, range 0-127
  51, 0x00, 0x00, //Knob 51 : CC, range 0-127
  52, 0x00, 0x00, //Knob 52 : CC, range 0-127
  53, 0x00, 0x00, //Knob 53 : CC, range 0-127
  54, 0x00, 0x00, //Knob 54 : CC, range 0-127
  55, 0x00, 0x00, //Knob 55 : CC, range 0-127
  56, 0x00, 0x00, //Knob 56 : CC, range 0-127
  57, 0x00, 0x00, //Knob 57 : CC, range 0-127
  58, 0x00, 0x00, //Knob 58 : CC, range 0-127
  59, 0x00, 12, //Knob 59 : CC, range 0-127

  /*        inversion bits                 */
  0, 0, 0, 0, 0, 0, 0, 0,
  0

};


uint8_t knobBuffer[4][60];

uint8_t emittedValue[3][60];

uint8_t channel;


struct Knob_t {
  uint8_t CC;
  uint8_t NRPN;
  uint8_t SYSEX;
};

Knob_t knobInfo[NUMBEROFKNOBS];

uint8_t currentPreset;

unsigned long lastButtonPress;  //stores the timestamp of the last button press

uint64_t invertBits;

bool dropNRPNMSBvalue;

#endif
