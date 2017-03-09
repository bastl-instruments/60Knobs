/*
   This file includes all the code associated to the prototypes defined in "functions.h"
*/

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

//Averages the knob position and decides if a message should be outputed, if yes, decodes the current preset and sends the acording message
void interpretKnob(uint8_t index, bool force, bool inhibit) {
  uint8_t knobSpec[3];  //holds the preset preferences for that knob

  //we average the values in the buffers
  uint16_t toSend = 0;
  for (int i = 0; i < 4; i++) {
    toSend += knobBuffer[i][index];
  }
  toSend /= 4;

  //we flip the value of the knob if it is in "inverted" mode
  if (isInverted(index) != 0) {
    toSend = 127 - toSend;
  }

  //if the value to send is relevant, we send it to the MIDI OUT port
  if (((toSend != emittedValue[0][index]) && (toSend != emittedValue[1][index]) && (toSend != emittedValue[2][index])) || (force == true)) {  //if a message should be sent

    //look at the knobInfo array to know how we should format the data
    knobSpec[0] = knobInfo[index * 3];  //the CC byte
    knobSpec[1] = knobInfo[index * 3 + 1]; //the NRPN byte
    knobSpec[2] = knobInfo[index * 3 + 2]; //the sysex byte

    //First we detect which kind of knob it is and we emit the data accordingly
    if (bitRead(knobSpec[2], 7) == 1) {
      if (knobSpec[1] == 0) {
        /*---   It's a CC knob    ---*/
        if (!inhibit) {
          //check the channel of that specific knob
          uint8_t knobChannel = knobSpec[2] & 0x7f;
          if (knobChannel > 0 && knobChannel < 17) {
            MIDI.sendControlChange(knobSpec[0] & 0x7f, toSend, knobChannel);
          }
          else if (knobChannel == 0) { //if the channel number is 0, the CC will be sent on the global channel
            MIDI.sendControlChange(knobSpec[0], toSend, channel);
          }
        }
      }
      else {
        /*---   It's an NRPN knob    ---*/
        //we calculate the range of the current knob
        uint8_t range = knobSpec[2] & 0x7f;

        if (!inhibit) {
          if ((knobSpec[1] & 0x80) && (knobSpec[0] & 0x80)) //if the knob is Unipolar NRPN (range : 0~+Max)
          {
            sendUnipolarNRPN(knobSpec[1], knobSpec[0], map(toSend, 0, 127, 0, range), channel);
          }
          else {  //the knob is Bipolar NRPN (range : -63~+63)
            if (range < 64) sendBipolarNRPN(knobSpec[1], knobSpec[0], map(toSend, 0, 127, -range, range), channel);
            else if ( range <= 64 + 4) {
              switch (range) {
                case 64 :
                  sendExtendedNRPN(knobSpec[1], knobSpec[0], map(toSend, 0, 127, 0, 164), channel);
                  break;
                case 65 :
                  sendExtendedNRPN(knobSpec[1], knobSpec[0], map(toSend, 0, 127, 0, 200), channel);
                  break;
                case 66 :
                  sendExtendedNRPN(knobSpec[1], knobSpec[0], map(toSend, 0, 127, 0, 1600), channel);
                  break;
                case 67 :
                  sendExtendedNRPN(knobSpec[1], knobSpec[0], map(toSend, 0, 127, 0, 2000), channel);
                  break;
              }
            }
          }
        }
      }
    }
    else {
      /*---   It's an DX7 SysEx knob    ---*/
      sendDX7Message(knobSpec[1], knobSpec[2], toSend);
    }

    //we fill the emission buffers
    for (int i = 2; i > 0; i--) {
      emittedValue[i][index] = emittedValue[i - 1][index];
    }
    emittedValue[0][index] = toSend;
  }
}

//Sends an Unipolar NRPN message
void sendUnipolarNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, uint8_t value, uint8_t channel) {
  MIDI.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); //NRPN Number LSB
  MIDI.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);  //NRPN Number MSB
  MIDI.sendControlChange(6, value, channel);  //NRPN Value
  if(!dropNRPNMSBvalue) {
  MIDI.sendControlChange(38, 0, channel);  //NRPN Value
  }
}

//Sends a Bipolar NRPN message
void sendBipolarNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int8_t value, uint8_t channel) {
  MIDI.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); //NRPN Number LSB
  MIDI.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);  //NRPN Number MSB

  //if the value to send is negative, format the data in the right way
  if (value < 0) {
    MIDI.sendControlChange(6, 1, channel);  //NRPN MSB negative
    MIDI.sendControlChange(38, 128 + value, channel); //NRPN MSB negative
  }
  //if the value is positive
  else {
    MIDI.sendControlChange(38, value, channel);
  }
}

void sendExtendedNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int16_t value, uint8_t channel) {
  MIDI.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); //NRPN Number LSB
  MIDI.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);  //NRPN Number MSB
  MIDI.sendControlChange(6, (value>>8) & 0x7f, channel);  //NRPN MSB
  MIDI.sendControlChange(38, value & 0x7f, channel); //NRPN MSB
}

//Sends a DX7-Parameter Change formated Sysex message
void sendDX7Message(uint8_t paramNBR, uint8_t rangeMax, uint8_t value) {
  //the array to transmit
  uint8_t data[5] = {0x43, 0x10, 0x00, 0x00, 0x00};

  //if the parameter number takes more than seven bits, set the MSBit correctly
  if (paramNBR > 127) {
    data[2] = 0x01;
  }

  data[3] = paramNBR & 0x7f;
  data[4] = map(value, 0, 127, 0, rangeMax);

  //output the Sysex message
  MIDI.sendSysEx(5, data, false);

  //sync all the voices
  MIDI.sendControlChange(127, 0, 1);  //cc 127 on channel 1 (DX7 only sensitive on channel 1)
}

//Loads the specified preset in the RAM and make it the last used preset
void loadPreset(uint8_t presetNumber) {
  uint16_t baseAddress = presetNumber * (PRESET_LENGTH - 1) + 1;

  //We load the global MIDI channel of the selected preset
  channel = EEPROM.read(baseAddress);

  //We can now copy the knobsDescriptors in RAM
  for (uint8_t i = 0; i < KNOB_DESCRIPTOR_LENGTH; i++) {
    knobInfo[i] = EEPROM.read(baseAddress + 1 + i);
  }

  //and finally load the inversion bits
  invertBits = 0;
  for (int i = 0; i < INVERTBITS_LENGTH; i++) {
    uint64_t eepromValue = (uint64_t)(EEPROM.read(baseAddress + KNOB_DESCRIPTOR_LENGTH + 1 + i));
    invertBits |= eepromValue << (i * 8);
  }

  dropNRPNMSBvalue = EEPROM.read(baseAddress + KNOB_DESCRIPTOR_LENGTH + INVERTBITS_LENGTH + 1);

  //update the last used preset
  EEPROM.update(0, presetNumber);
}


//Is executed everytime a Sysex message is received
void sysExInterpreter(byte* data, unsigned messageLength) {

  //check if this is a valid Sysex message
  if (data[MANUFACTURER] == BASTL) {
    //check the command byte and acts accordingly
    switch (data[COMMAND]) {

      case SETKNOBASGLOBALCC :  //Sets a knob as a global CC knob
        {
          //PARAM 1 : which knob do we affect ?
          //PARAM 2 : the CC number
          if (data[PARAM1] < 60) {
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = data[PARAM2];
            knobInfo[knobPointer + 1] = 0;
            knobInfo[knobPointer + 2] = 128;

            //knob in normal mode by default
            clearBits64(invertBits, data[PARAM1]);
          }

          break;
        }

      case SETKNOBASINDEPCC :   //Sets a knob as an independent CC knob
        {
          //PARAM 1 : which knob do we affect ?
          //PARAM 2 : CC number
          //PARAM 3 : the MIDI channel of that knob
          if (data[PARAM1] < 60) {
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = data[PARAM2];
            knobInfo[knobPointer + 1] = 0;
            knobInfo[knobPointer + 2] = data[PARAM3] | 0x80;

            //knob in normal mode by default
            clearBits64(invertBits, data[PARAM1]);
          }

          break;
        }

      case DISABLEKNOB :  //Sets a knob as an inactive CC knob
        {
          //PARAM 1 : which knob do we affect ?
          if (data[PARAM1] < 60) {
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = 0;  //bullshit CC
            knobInfo[knobPointer + 1] = 0;
            knobInfo[knobPointer + 2] = 17 | 0x80; //out of range -> knob disabled
          }

          break;
        }

      case SETKNOBASBNRPN : //Sets a knob as a Bipolar NRPN (-63, +63) knob
        {
          //PARAM 1 : which knob do we affect ?
          //PARAM 2 : NRPN number LSB
          //PARAM 3 : NRPN number MSB
          //PARAM 4 : NRPN range (-range to +range), range max : 63
          if (data[PARAM1] < 60) {
            uint8_t range = data[PARAM4];
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = data[PARAM2];
            knobInfo[knobPointer + 1] = data[PARAM3] | 0x80;
            if (range > 63) range = 63;
            knobInfo[knobPointer + 2] = 128 + range;

            //knob in normal mode by default
            clearBits64(invertBits, data[PARAM1]);
          }

          break;
        }

      case SETKNOBASUNRPN : //Sets a knob as a Unipolar NRPN (0, +127) knob
        {
          //PARAM 1 : which knob do we affect ?
          //PARAM 2 : NRPN number LSB
          //PARAM 3 : NRPN number MSB
          //PARAM 4 : NRPN range (0 to +range), max range : 127
          if (data[PARAM1] < 60) {
            uint8_t range;
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = data[PARAM2] | 0x80;
            knobInfo[knobPointer + 1] = data[PARAM3] | 0x80;
            knobInfo[knobPointer + 2] = 128 + data[PARAM4];

            //knob in normal mode by default
            clearBits64(invertBits, data[PARAM1]);
          }

          break;
        }


      //for the ranges +164, +200, +1600 and +2000
      case SETKNOBASENRPN : //Sets a knob as an Extended Unipolar NRPN (0, XX) knob
        {
          //PARAM 1 : which knob do we affect ?
          //PARAM 2 : NRPN number LSB
          //PARAM 3 : NRPN number MSB
          //PARAM 4 : NRPN range (1 : +164, 1 : +200, 1 : +1600, 1 : +2000)
          if (data[PARAM1] < 60) {
            uint8_t range = data[PARAM4];
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = data[PARAM2];
            knobInfo[knobPointer + 1] = data[PARAM3] | 0x80;
            switch (range) {
              case 1 :
                range = 63 + 1; //+164
                break;
              case 2 :
                range = 63 + 2; //+200
                break;
              case 3 :
                range = 63 + 3; //+1600
                break;
              case 4 :
                range = 63 + 4; //+2000
                break;
              default :
                range = 63 + 10; //wrong number -> no action
                break;
            }
            knobInfo[knobPointer + 2] = 128 + range;

            //knob in normal mode by default
            clearBits64(invertBits, data[PARAM1]);
          }

          break;
        }

      case SETKNOBASDX :  //Sets a knob as a DX7 parameter change knob
        {
          //PARAM 1 : which knob do we affect ?
          //PARAM 2 : DX7 parameter number most significant bit
          //PARAM 3 : DX7 parameter number last 7 bits
          //PARAM 4 : maximum value that can be reached by that parameter
          if (data[PARAM1] < 60) {
            uint8_t knobPointer = data[PARAM1] * 3;
            knobInfo[knobPointer] = 0;
            knobInfo[knobPointer + 1] = (data[PARAM2] << 7) | data[PARAM3];
            knobInfo[knobPointer + 2] = data[PARAM4];

            //knob in normal mode by default
            clearBits64(invertBits, data[PARAM1]);
          }

          break;
        }

      case INVERTKNOB : //Sets a knob to be inverted or not
        {
          //PARAM1 : which knob do we affect ?
          //PARAM2 : 0-> knob in normal mode; 1-> knob in invert mode
          if (data[PARAM1] < 60) {
            if (data[PARAM2] == 0) {
              clearBits64(invertBits, data[PARAM1]);
            }
            else {
              invertBits = invertBits | (((uint64_t)1) << data[PARAM1]);
              //bitSet(invertBits, data[PARAM1]);
            }
          }
          break;
        }


      case DROPNRPNMSB :
        {
          if(data[PARAM1] > 0) {
            dropNRPNMSBvalue = true;        
          } else {
            dropNRPNMSBvalue = false;
          }
        }

      case PRESETSAVE : //Saves the current state of the machine to the specified slot
        {
          //PARAM 1 : the slot where the preset will be saved

          if (data[PARAM1] < 5) {
            savePreset(data[PARAM1]);
          }
          break;
        }

      case PRESETLOAD : //Sets the state of the machine according to the specified preset
        {
          //PARAM 1 : number of the preset to load

          if (data[PARAM1] < 5) {
            loadPreset(data[PARAM1]);
          }

          break;
        }

      case SYNCKNOBS :  //Forces the emission of the messages associated to every knob
        {
          //NO PARAM
          for (uint8_t i = 0; i < 60; i++) {
            interpretKnob(i, true, false);
          }

          break;
        }

      case CHANNELCHANGE :  //Changes the global MIDI channel according to the one specified
        {
          //PARAM 1 : MIDI channel  (1 to 16)
          if (data[PARAM1] < 17 && data[PARAM1] > 0) {
            channel = data[PARAM1];
          }

          break;
        }

      case RANDOMIZER : //Sends random CC messages forthe CC range specified
        {
          //PARAM 1 : Min CC number
          //PARAM 2 : Max CC number
          if (data[PARAM1] < 127 && data[PARAM2] >= data[PARAM1]) {
            for (int i = data[PARAM1]; i <= data[PARAM2]; i++) {
              MIDI.sendControlChange(i, random(0, 127), channel);
            }

          }
          break;
        }
    }
  }
}

//Is executed everytime a MIDI Program Change message is received
//loads the specified preset
void handleProgramChange(byte channel, byte number) {
  if (number < 5) {
    loadPreset(number);
    currentPreset = number;
  }
}

//Handles the "menu" system, what to do when the button is pressed
void renderFunctionButton() {
  if (!digitalRead(BUTTON_PIN)) {

    //We dont want any messages to be transmitted while the button is down
    MIDI.turnThruOff();

    //check if the button has been double pressed, if yes : force the emission of the messages associated to each knob
    if ((abs(millis() - lastButtonPress) > 100) && (abs(millis() - lastButtonPress) < 350)) {
      //show the user that the double click worked
      digitalWrite(LED_PIN, LOW);
      //sync the knobs
      for (uint8_t i = 0; i < 60; i++) {
        interpretKnob(i, true, false);
      }
      delay(500); //just for the hell of it, more visual perhaps
    }

    //we send an empty sysex message to ensure the connection between the 60K and the target
    MIDI.sendSysEx(0, NULL, false);

    //update the timing variable for the double press detection
    lastButtonPress = millis();

    while (!digitalRead(BUTTON_PIN)) { //while the button is held down

      //we detect the movement on the knobs we care for (0 to 15 and 50 to 54)
      for (int currentKnob = 0; currentKnob < 60; currentKnob++) {
        MIDI.read();
        selectKnob(currentKnob);  //Sets up the MUXs to direct the right knob to the analog input
        MIDI.read();
        updateKnob(currentKnob);  //fills the buffer, to know which knobs have moved
      }


      //do we need to change the channel ?
      for (uint8_t channelKnob = 0; channelKnob < 16; channelKnob++) {
        //if one the knobs associated to the MIDI channel selection has moved enough
        if (abs(knobBuffer[0][channelKnob] - knobBuffer[1][channelKnob]) > THRESHOLD) {
          channel = channelKnob + 1;
          //give a visual feedback to prove that the channel has changed
          digitalWrite(LED_PIN, LOW);
          delay(100);
          digitalWrite(LED_PIN, HIGH);
        }
      }

      //do we need to change preset ?
      for (uint8_t presetKnob = 50; presetKnob < 55; presetKnob++) {
        //if one of the knobs associated with the preset selection has moved enough
        if (abs(knobBuffer[0][presetKnob] - knobBuffer[1][presetKnob]) > THRESHOLD) {
          currentPreset = presetKnob - 50;
          loadPreset(currentPreset);
          //give a visual feedback to prove that the preset has changed
          digitalWrite(LED_PIN, LOW);
          delay(250);
          digitalWrite(LED_PIN, HIGH);
        }
      }

      //we exit the "menu"so we can turn the MIDI thru back on
      MIDI.turnThruOn();
    }
  }
}


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


void savePreset(uint8_t presetNbr)
{
  uint16_t baseAddress = presetNbr * (PRESET_LENGTH - 1) + 1;

  EEPROM.update(baseAddress, channel); //write the channel in the EEPROM

  //write the knobs descriptors in the EEPROM
  for (int i = 0; i < KNOB_DESCRIPTOR_LENGTH; i++) {
    EEPROM.update(baseAddress + 1 + i, knobInfo[i]);
  }

 

  //finally write the inversion bits to the EEPROM
  for (int i = 0; i < INVERTBITS_LENGTH; i++) {
    uint8_t oneByteValue = (invertBits >> (i * 8));
    EEPROM.update(baseAddress + KNOB_DESCRIPTOR_LENGTH + 1 + i, oneByteValue);
  }

   EEPROM.update(baseAddress + KNOB_DESCRIPTOR_LENGTH + INVERTBITS_LENGTH + 1, dropNRPNMSBvalue);

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

//return 0 if not inverted, not 0 otherwise
uint64_t isInverted(uint8_t index) {
  uint64_t offset = (uint64_t)index;
  return (invertBits & ((uint64_t)1 << offset)) >> offset;
}

void clearBits64(uint64_t & value, uint8_t index) { 
  value = value & ~(((uint64_t)1) << index);
}
