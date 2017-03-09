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
  //we write the first default preset to the corresponding slots
  for (int address = 1; address < PRESET_LENGTH; address++) {
    EEPROM.update(address, factory_preset_1[address - 1]);
  }

  //we repeat that stage for the 4 remaining presets
  //we write the signature so that the device will never rewrite the factory presets
  EEPROM.update(EEPROM.length() - 3, 0xB0);
  EEPROM.update(EEPROM.length() - 2, 0x0B);
  EEPROM.update(EEPROM.length() - 1, 0x1E);
}

//Loads the specified preset in the RAM and make it the last used preset
void loadPreset(uint8_t presetNumber) {
  uint16_t baseAddress = presetNumber * (PRESET_LENGTH - 1) + 1;

  //We load the global MIDI channel of the selected preset
  channel = EEPROM.read(baseAddress);

  //We can now copy the knobsDescriptors in RAM
  for (uint8_t i = 0; i < sizeof(knobInfo); i++) {
    ((uint8_t*)knobInfo)[i] = EEPROM.read(baseAddress + 1 + i);
  }

  //and finally load the inversion bits
  invertBits = 0;
  for (int i = 0; i < sizeof(invertBits); i++) {
    uint64_t eepromValue = (uint64_t)(EEPROM.read(baseAddress + sizeof(knobInfo) + 1 + i));
    invertBits |= eepromValue << (i * 8);
  }

  dropNRPNMSBvalue = EEPROM.read(baseAddress + sizeof(knobInfo) + sizeof(invertBits) + 1);

  //update the last used preset
  EEPROM.update(0, presetNumber);
}



void savePreset(uint8_t presetNbr)
{
  uint16_t baseAddress = presetNbr * (PRESET_LENGTH - 1) + 1;

  EEPROM.update(baseAddress, channel); //write the channel in the EEPROM

  //write the knobs descriptors in the EEPROM
  for (int i = 0; i < sizeof(knobInfo); i++) {
    EEPROM.update(baseAddress + 1 + i, ((uint8_t*)knobInfo)[i]);
  }

 

  //finally write the inversion bits to the EEPROM
  for (int i = 0; i < sizeof(invertBits); i++) {
    uint8_t oneByteValue = (invertBits >> (i * 8));
    EEPROM.update(baseAddress + sizeof(knobInfo) + 1 + i, oneByteValue);
  }

   EEPROM.update(baseAddress + sizeof(knobInfo) + sizeof(invertBits) + 1, dropNRPNMSBvalue);

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
