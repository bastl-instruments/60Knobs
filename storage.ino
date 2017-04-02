#include "functions.h"
#include "definitions.h"

//checks if the machine starts up for the first time and loads the factory presets if needed
uint8_t firstStartupCheck() {
  //we check if this is the first startup of the machine
  uint8_t byte1, byte2, byte3;

  byte1 = EEPROM.read(EEPROM.length() - 3);
  byte2 = EEPROM.read(EEPROM.length() - 2);
  byte3 = EEPROM.read(EEPROM.length() - 1);

  //if this is the first time this device is powered on
  if ((byte1 != 0xB0) && (byte2 != 0x0B) && (byte3 != 0x1E)) {  //"boobie" signature yay !
    return 1;
  }
  else return 0;
}

void formatFactory() {
  
  //First program the Last Used Preset : the first one
  EEPROM.update(0, 0x01);
  
  // Create default preset
  Preset_t defaultPreset;
  defaultPreset.channel = 1;
  for (uint8_t i=0; i<NUMBEROFKNOBS; i++) {
    defaultPreset.knobInfo[i].CC = 0;
    defaultPreset.knobInfo[i].NRPN = 0;
    defaultPreset.knobInfo[i].SYSEX = 0;
  }
  defaultPreset.invertBits = 0;
  defaultPreset.dropNRPNLSBvalue = 0;
  
  // we write the first default preset to the corresponding slots
  // write the active preset to EEPROM; byte by byte
  uint16_t baseAddress = 1;  
  for (uint16_t byteIndex=0; byteIndex<sizeof(Preset_t); byteIndex++) {
    EEPROM.update(baseAddress + byteIndex, ((uint8_t*)(&defaultPreset))[byteIndex]);
  }

  //we repeat that stage for the 4 remaining presets
  // to be done...
  
  
  //we write the signature so that the device will never rewrite the factory presets
  EEPROM.update(EEPROM.length() - 3, 0xB0);
  EEPROM.update(EEPROM.length() - 2, 0x0B);
  EEPROM.update(EEPROM.length() - 1, 0x1E);
}


//Loads the specified preset in the RAM and make it the last used preset
void loadPreset(uint8_t presetNumber) {
  
  uint16_t baseAddress = presetNumber * sizeof(Preset_t) + 1;

  // read the active preset from EEPROM; byte by byte  
  for (uint16_t byteIndex=0; byteIndex<sizeof(Preset_t); byteIndex++) {
    ((uint8_t*)(&activePreset))[byteIndex] = EEPROM.read(baseAddress + byteIndex);
  }

  //update the last used preset
  EEPROM.update(0, presetNumber);
}



void savePreset(uint8_t presetNbr)
{
  uint16_t baseAddress = presetNbr * sizeof(Preset_t) + 1;

  // write the active preset to EEPROM; byte by byte  
  for (uint16_t byteIndex=0; byteIndex<sizeof(Preset_t); byteIndex++) {
    EEPROM.update(baseAddress + byteIndex, ((uint8_t*)(&activePreset))[byteIndex]);
  }
  
  //Visual feedback
  //we wait a bit with the LED oon
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  //we blink it
  digitalWrite(LED_PIN, LOW);
  delay(250);
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
}
