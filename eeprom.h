#ifndef EEPROM_SETUP
#define EEPROM_SETUP

#include "Arduino.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512

class EEPROMSetup
{
  private:
    char _dataRead[150];
    char _dataWrite[150];

  public:
    void initSetup();
    
    char* getDataRead();
    char* getDataWrite();

    void eepromRead(int address, char variable[150]);
    void eepromWrite(int address, char variable[150]);

};

#endif