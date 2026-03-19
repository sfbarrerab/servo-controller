#pragma once
#include <Arduino.h>
#include <EEPROM.h> 
#include <servomotor.h>

#define EEPROM_BASE_ADDR 0
#define EEPROM_SIZE 512

void initEEPROM();
void readEEPROM();
void writeEEPROM(uint8_t servoId, uint16_t firstPos, uint16_t secondPos);