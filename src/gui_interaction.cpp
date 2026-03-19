#include <gui_interaction.h>
void interact_with_gui() {
  if (Serial.available()) {

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("GET")) {

      int firstComma = cmd.indexOf(',');
      int servoIdParsed = 0;

      if (firstComma > 0) {
        servoIdParsed = cmd.substring(firstComma + 1).toInt();
        if (servoIdParsed < 0 || servoIdParsed >= NUMBER_OF_SERVOS)
          return;
      }

      uint8_t servoId = servoIdParsed;

      Serial.print(servoId);
      Serial.print(",");
      Serial.print(getFirstPosition(servoId));
      Serial.print(",");
      Serial.println(getSecondPosition(servoId));
    }

    else if (cmd.startsWith("SET")) {

      int firstComma = cmd.indexOf(',');
      int secondComma = cmd.indexOf(',', firstComma + 1);
      int thirdComma = cmd.indexOf(',', secondComma + 1);

      if (firstComma < 0 || secondComma < 0 || thirdComma < 0)
        return;

      int servoIdParsed = cmd.substring(firstComma + 1, secondComma).toInt();
      if (servoIdParsed < 0 || servoIdParsed >= NUMBER_OF_SERVOS)
        return;

      uint8_t servoId = servoIdParsed;

      uint16_t newFirst = cmd.substring(secondComma + 1, thirdComma).toInt();
      uint16_t newSecond = cmd.substring(thirdComma + 1).toInt();

      if (newFirst < PULSE_MIN || newFirst > PULSE_MAX) return;
      if (newSecond < PULSE_MIN || newSecond > PULSE_MAX) return;

      setFirstPosition(servoId, newFirst);
      setSecondPosition(servoId, newSecond);

      writeEEPROM(servoId, newFirst, newSecond);

      Serial.println("OK");
    }
  }
}