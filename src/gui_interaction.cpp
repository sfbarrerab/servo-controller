#include <gui_interaction.h>


void interact_with_gui(){
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "GET") {
      Serial.print(initPos);
      Serial.print(",");
      Serial.println(finalPos);
    }
    else if (cmd.startsWith("SET")) {
      int firstComma = cmd.indexOf(',');
      int secondComma = cmd.indexOf(',', firstComma + 1);

      initPos = cmd.substring(firstComma + 1, secondComma).toInt();
      finalPos = cmd.substring(secondComma + 1).toInt();

      Serial.println("OK");
    }
  }

}