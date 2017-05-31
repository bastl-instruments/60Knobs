/* EXTERNAL LIBRARIES */
#include <MIDI.h>
#include <EEPROM.h>

/* OTHER CUSTOM FILES */
#include "definitions.h"
#include "variables.h"

/*---   PIN DEFINITION   ---*/
#define MIDI_TX_PIN 1
#define MAIN_MUX_S0 2
#define MAIN_MUX_S1 3
#define MAIN_MUX_S2 4
#define SECONDARY_MUX_S0 5
#define SECONDARY_MUX_S1 6
#define SECONDARY_MUX_S2 7
#define LED_PIN 18
#define BUTTON_PIN 19


MIDI_CREATE_DEFAULT_INSTANCE();

/*---   INIT CODE   ---*/
void setup() {
  
  // Setup the pins
  pinMode(MIDI_TX_PIN, OUTPUT);
  digitalWrite(MIDI_TX_PIN, HIGH);  //make sure no random messages are sent to the MIDI port during init
  pinMode(MAIN_MUX_S0, OUTPUT);
  pinMode(MAIN_MUX_S1, OUTPUT);
  pinMode(MAIN_MUX_S2, OUTPUT);
  pinMode(SECONDARY_MUX_S0, OUTPUT);
  pinMode(SECONDARY_MUX_S1, OUTPUT);
  pinMode(SECONDARY_MUX_S2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //if this is the first time the device is powered on, we write the factory presets in the memory
  if(!isEEPROMvalid()) {
    formatFactory();
  }

  //Load the last used preset as stored in EEPROM
  loadPreset(EEPROM.read(lastUsedPresetAddress));

  //We initialise the button press monitoring system
  lastButtonPress = millis();

  //We don't want any incorrect data sent at startup so we fill the buffers
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < 60; i++) {
      selectKnob(i);  //select the knob
      updateKnob(i);  //update the buffers
      interpretKnob(i, false, true);  //fill the emission buffers but do not send midi data
    }
  }

  //Midi configuration
  MIDI.setHandleSystemExclusive(sysExInterpreter);  //we set the callbacks
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.turnThruOn();
  MIDI.begin();

  //We send an Active Sensing MIDI message to notify the target that the controller is on the bus
  MIDI.sendRealTime((midi::MidiType) 0xFE);
}

/*---   MAIN LOOP   ---*/
//all the MIDI.read() statements are here to reduce the latency of the device
void loop() {
  
  // we turn the LED on
  digitalWrite(LED_PIN, HIGH);

  // we scan the knobs one by one and send the corresponding data
  for (int currentKnob = 0; currentKnob < 60; currentKnob++) {
    MIDI.read();
    selectKnob(currentKnob);  //Sets up the MUXs to direct the right knob to the analog input
    MIDI.read();
    updateKnob(currentKnob);  //Read the current knob value and update the buffers
    MIDI.read();
    interpretKnob(currentKnob, false, false); //send the data if needed
  }

  //perform the function buttons-related actions
  renderFunctionButton();
}
