
#include "functions.h"
#include "definitions.h"


//Sets up the MUXs in order to select the specified knob
void selectKnob(uint8_t index) {
  //get the configuration of the MUXs according to the lookup table
  uint8_t knobID = knobLUT[index];

  //we setup the main MUX
  digitalWrite(MAIN_MUX_S0, bitRead(knobID, 3));
  digitalWrite(MAIN_MUX_S1, bitRead(knobID, 4));
  digitalWrite(MAIN_MUX_S2, bitRead(knobID, 5));

  //we setup the secondary MUXs
  digitalWrite(SECONDARY_MUX_S0, bitRead(knobID, 0));
  digitalWrite(SECONDARY_MUX_S1, bitRead(knobID, 1));
  digitalWrite(SECONDARY_MUX_S2, bitRead(knobID, 2));

  MIDI.read();

  //we wait a bit
  delayMicroseconds(100);
}

//Updates the buffers and samples the analog pin
void updateKnob(uint8_t index) {
  for (int i = 3; i > 0; i--) {
    knobBuffer[i][index] = knobBuffer[i - 1][index];
  }
  knobBuffer[0][index] = analogRead(A0) >> 3;
}
