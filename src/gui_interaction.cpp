#include <gui_interaction.h>

void interact_with_gui() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();   

    if (cmd.startsWith("GET")) {
      // Parse GET,<servoId>
      int firstComma = cmd.indexOf(',');
      int servoId = 0; // default
      if (firstComma > 0) {
        servoId = cmd.substring(firstComma + 1).toInt();
        if (servoId < 0 || servoId >= NUMBER_OF_SERVOS) return; // invalid servo
      }

      Serial.print(servoId);
      Serial.print(",");
      Serial.print(getFirstPosition(servoId));
      Serial.print(",");
      Serial.println(getSecondPosition(servoId));
    } 
    else if (cmd.startsWith("SET")) {
      // Format: SET,<servoId>,<init>,<final>
      int firstComma = cmd.indexOf(',');
      int secondComma = cmd.indexOf(',', firstComma + 1);
      int thirdComma = cmd.indexOf(',', secondComma + 1);

      if (firstComma < 0 || secondComma < 0 || thirdComma < 0) return; // invalid

      int servoId = cmd.substring(firstComma + 1, secondComma).toInt();
      if (servoId < 0 || servoId >= NUMBER_OF_SERVOS) return; // invalid

      int newFirst = cmd.substring(secondComma + 1, thirdComma).toInt();
      int newSecond = cmd.substring(thirdComma + 1).toInt();

      setFirstPosition(servoId,newFirst);
      setSecondPosition(servoId,newSecond); 
      Serial.println("OK");
      writeEEPROM(servoId, newFirst, newSecond);
    }
  }
}