#include "eeprom.h"

void EEPROMSetup::initSetup()
{
  EEPROM.begin(EEPROM_SIZE);
}

char* EEPROMSetup::getDataRead() {
  return _dataRead;
}

char* EEPROMSetup::getDataWrite() {
  return _dataWrite;
}


void EEPROMSetup::eepromRead(int address, char variable[150])
{
  EEPROM.get(address, variable);
}

void EEPROMSetup::eepromWrite(int address, char variable[150])
{
  EEPROM.put(address, variable);
}