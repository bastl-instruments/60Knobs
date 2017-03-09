/*
 * This file contains all the prototypes of the functions used throughout the code
 */

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdint.h"
#include "stdbool.h"

#define byte uint8_t

void selectKnob(uint8_t index); //Sets up the MUXs in order to select the specified knob
void updateKnob(uint8_t index); //Updates the buffers and samples the analog pin
uint16_t getKnobValue(uint8_t knobIndex);
void interpretKnob(uint8_t index, bool force, bool inhibit);  //Averages the knob position and decides if a message should be outputed, if yes, decodes the current preset and sends the acording message
void sendUnipolarNRPN(uint16_t NRPNNumber, uint8_t value, uint8_t channel);
void sendBipolarNRPN(uint16_t NRPNNumber, int8_t value, uint8_t channel);
void sendExtendedNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int16_t value, uint8_t channel);
void sendDX7Message(uint8_t paramNBR, uint8_t rangeMax, uint8_t value);
void loadPreset(uint8_t presetNumber);
void blackMidiLoop();
void sysExInterpreter(byte* data, unsigned length);
void handleProgramChange(byte channel, byte number);
void renderFunctionButton();
uint64_t isInverted(uint8_t index);
void savePreset(uint8_t presetNbr);
uint8_t firstStartupCheck();
void formatFactory();
void clearBits64(uint64_t & value, uint8_t index);

#endif
