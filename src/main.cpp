#include <Arduino.h>
#include <servomotor.h> // servo functions 
//#include <memory_mgmt.h> // memory functions
#include <gui_interaction.h> // Functions to interact with the gui

void setup() {
  // serial communication
  Serial.begin(9600);
  delay(1000);
  initServoControlPins();
}

void loop() {
  // set and get positions from GUI
  interact_with_gui();
  // React when servos control pin change
  moveServoWithControlPins();
  delay(100);
}


