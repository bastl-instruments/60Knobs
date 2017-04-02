#ifndef _VARIABLES_H_
#define _VARIABLES_H_

/*
*  A preset stores all the information that
*  define how the device operates
*/
struct Preset_t {
  
  // channel the device is sending on
  uint8_t channel;
  
  // information on how knobs are setup
  struct Knob_t {
    uint8_t CC;
    uint8_t NRPN;
    uint8_t SYSEX;
  } knobInfo[NUMBEROFKNOBS];
  
  // compressed bitarray also for knobs setup
  uint64_t invertBits;
  
  // should NRPN LSB be not sent if it is zero
  bool dropNRPNLSBvalue;
};



/* Device setup data */

uint8_t currentPresetNumber;
Preset_t activePreset;



/* Hardware data */

const uint8_t knobLUT[60] =
{ 42, 58, 16, 8, 0, 24, 40, 56, 48, 32, 50, 34, 19, 11, 3, 27, 43, 59, 51, 35, 17, 9, 21, 13, 5, 29, 45, 61,
  53, 37, 1, 25, 23, 15, 7, 31, 47, 63, 55, 39, 41, 57, 22, 14, 6, 30, 46, 62, 54, 38, 49, 33, 20, 12, 4, 28, 44, 60, 52, 36
};

uint8_t knobBuffer[4][60];
uint8_t emittedValue[3][60];
unsigned long lastButtonPress;  //stores the timestamp of the last button press

#endif
