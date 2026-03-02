#include <Arduino.h>
#include <servomotor.h> // servo functions 
//#include <memory_mgmt.h> // memory functions
#include <gui_interaction.h> // Functions to interact with the gui

int currentRead_PortND;
int previousRead_PortND;

// Set initial position
int initPos = 600;
int finalPos = 900;

void setup() {
  // serial communication
  Serial.begin(9600);
  initServoControlPins();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // set and get positions from GUI
  interact_with_gui();
  // React when servos control pin change
  moveServoWithControlPins();
  delay(100);
}


