/*
 * This file contains all the definitions of the constats used throughout the code
 */

#include <stdint.h>

/*---   SYSEX INTERPRETER CONSTANTS   ---*/
#define BASTL 48
#define MANUFACTURER 1
#define COMMAND 2
#define PARAM1 3
#define PARAM2 4
#define PARAM3 5
#define PARAM4 6

//the numbers associated with every command in the interpreter
#define SETKNOBASGLOBALCC 1
#define SETKNOBASINDEPCC 15
#define SETKNOBASBNRPN 2
#define SETKNOBASUNRPN 3
#define SETKNOBASDX 4
#define PRESETSAVE 5
#define PRESETLOAD 6
#define DUMPPRESET 7
#define SYNCKNOBS 8
#define CHANNELCHANGE 9
#define DISABLEKNOB 16
#define RANDOMIZER 10
#define INVERTKNOB 17
#define DROPNRPNMSB 19
#define SETKNOBASENRPN 18

/*---   FUNCTION BUTTON CONSTANTS   ---*/
#define THRESHOLD 2
#define CLICKSPEEDMIN 100
#define CLICKSPEEDMAX 350

/*---   PRESET MANAGEMENT CONSTANTS   ---*/
#define PRESET_LENGTH 191
#define INVERTBITS_LENGTH 8
#define KNOB_DESCRIPTOR_LENGTH 180

/*---   MISC. CONSTANTS   ---*/
#define NUMBEROFKNOBS 60



