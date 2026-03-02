#include <Arduino.h>
#include <servomotor.h> // servo functions 
//#include <memory_mgmt.h> // memory functions
#include <gui_interaction.h> // Functions to interact with the gui

int currentRead_PortND;
int previousRead_PortND;


// Set initial position
int initPos = 600;
int finalPos = 900;

// Declare servos
Servo servoOutput_ND;

// Pins to control the servo triggering to change position
#define CONTROL_PIN_SGN_0 7

void setup() {
  // serial communication
  Serial.begin(9600);
  // define Pins
  pinMode(CONTROL_PIN_SGN_0,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Default pin values
  previousRead_PortND = LOW;
}

void loop() {
  // set and get positions from GUI
  interact_with_gui();
  // Check the current lecture
  currentRead_PortND = digitalRead(CONTROL_PIN_SGN_0);

  // React only if the value changed
  if(previousRead_PortND != currentRead_PortND){
    if(digitalRead(CONTROL_PIN_SGN_0) == HIGH){
      digitalWrite(LED_BUILTIN, HIGH);
      moveServoSilent(servoOutput_ND, SERVO_OUTPUT_PIN_0, PULSE_MIN, PULSE_MAX,  initPos);
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      moveServoSilent(servoOutput_ND, SERVO_OUTPUT_PIN_0, PULSE_MIN, PULSE_MAX, finalPos);
    }
  }

  // Update the previous value
  previousRead_PortND = currentRead_PortND;
  delay(100);
}


