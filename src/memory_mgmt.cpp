#include <memory_mgmt.h>

void initEEPROM(){
  EEPROM.begin(EEPROM_SIZE);
  readEEPROM();
}

// Each servo reserves 4 bytes
// First position occupies the first 2 bytes...
uint16_t getFirstAddr(uint8_t servoId) {
  return EEPROM_BASE_ADDR + servoId * 4;
}

// ... second position occupies the next 2 bytes after first position
uint16_t getSecondAddr(uint8_t servoId) {
  return EEPROM_BASE_ADDR + servoId * 4 + 2;
}

// Read the stored values in the memory, and if empty, initialize with default values 600,900
void readEEPROM() {
  for (uint8_t i = 0; i < NUMBER_OF_SERVOS; i++) {

    uint16_t firstPos;
    uint16_t secondPos;

    EEPROM.get(getFirstAddr(i), firstPos);
    EEPROM.get(getSecondAddr(i), secondPos);

    // Validate values
    if (firstPos < PULSE_MIN || firstPos > PULSE_MAX)
      firstPos = 600;

    if (secondPos < PULSE_MIN || secondPos > PULSE_MAX)
      secondPos = 900;

    setFirstPosition(i, firstPos);
    setSecondPosition(i, secondPos);
  }
}

// Store the values in the memory
void writeEEPROM(uint8_t servoId, uint16_t firstPos, uint16_t secondPos) {
  EEPROM.put(getFirstAddr(servoId), firstPos);
  EEPROM.put(getSecondAddr(servoId), secondPos);
  EEPROM.commit();
}