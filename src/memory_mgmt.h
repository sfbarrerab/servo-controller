#pragma once
#include <Arduino.h>
#include <EEPROM.h> 
#include <servomotor.h>

#define EEPROM_BASE_ADDR 0

void readEEPROM();
void writeEEPROM(int servoId, int firstPos, int secondPos);