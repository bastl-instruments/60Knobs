/*
 * This file contains all the definitions of the constats used throughout the code
 */

#ifndef INC_GUARD_DEFINITIONS
#define INC_GUARD_DEFINITIONS

#include <stdint.h>

/*---   SYSEX INTERPRETER CONSTANTS   ---*/
const uint8_t BASTL_MANUFACTURER_ID = 48;

enum SysexByteNumber_t {
  MANUFACTURER = 1,
  COMMAND = 2,
  PARAM1 = 3,
  PARAM2 = 4,
  PARAM3 = 5,
  PARAM4 = 6
};

//the numbers associated with every command in the interpreter
enum Command_t {
  SETKNOBASGLOBALCC = 1,
  SETKNOBASINDEPCC = 15,
  SETKNOBASBNRPN = 2,
  SETKNOBASUNRPN = 3,
  SETKNOBASDX = 4,
  PRESETSAVE = 5,
  PRESETLOAD = 6,
  DUMPPRESET = 7,
  SYNCKNOBS = 8,
  CHANNELCHANGE = 9,
  DISABLEKNOB = 16,
  RANDOMIZER = 10,
  INVERTKNOB = 17,
  DROPNRPNMSB = 19,
  SETKNOBASENRPN = 18
};

/*---   FUNCTION BUTTON CONSTANTS   ---*/
#define THRESHOLD 2
#define CLICKSPEEDMIN 100
#define CLICKSPEEDMAX 350

/*---   PRESET MANAGEMENT CONSTANTS   ---*/
#define PRESET_LENGTH 191


/*---   MISC. CONSTANTS   ---*/
const uint8_t NUMBEROFKNOBS = 60;

#endif

