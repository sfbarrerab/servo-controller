#include <memory_mgmt.h>

int getFirstAddr(int servoId) {
  return EEPROM_BASE_ADDR + servoId * 4;
}

int getSecondAddr(int servoId) {
  return EEPROM_BASE_ADDR + servoId * 4 + 2;
}

void readEEPROM() {
  for (int i = 0; i < NUMBER_OF_SERVOS; i++) {

    int firstPos;
    int secondPos;

    EEPROM.get(getFirstAddr(i), firstPos);
    EEPROM.get(getSecondAddr(i), secondPos);

    // Validate values (important!)
    if (firstPos < PULSE_MIN || firstPos > PULSE_MAX)
      firstPos = 600;

    if (secondPos < PULSE_MIN || secondPos > PULSE_MAX)
      secondPos = 900;

    setFirstPosition(i, firstPos);
    setSecondPosition(i, secondPos);
  }
}

void writeEEPROM(int servoId, int firstPos, int secondPos) {
  EEPROM.put(getFirstAddr(servoId), firstPos);
  EEPROM.put(getSecondAddr(servoId), secondPos);
}