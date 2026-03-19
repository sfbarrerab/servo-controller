#pragma once
#include <ESP32Servo.h> // Arduino library
#include <Arduino.h>

#define NUMBER_OF_SERVOS 4

// the pulse width goes from 450 to 1050 microseconds
// being 750 microseconds the center position
#define PULSE_MIN   450
#define PULSE_MAX   1050
#define PULSE_CENTER 750
#define WORKING_FRERQUENCY 560

// Servo PWM outputs
#define SERVO_OUTPUT_PIN_0 18
#define SERVO_OUTPUT_PIN_1 19
#define SERVO_OUTPUT_PIN_2 21
#define SERVO_OUTPUT_PIN_3 22

// Control input pins
#define CONTROL_PIN_SGN_0 25
#define CONTROL_PIN_SGN_1 26
#define CONTROL_PIN_SGN_2 27
#define CONTROL_PIN_SGN_3 33

struct ServoControl {
  Servo servoHndl;
  uint8_t controlPin;
  int controlSignalValue = 0;
  uint8_t servoOutputPin;
  uint16_t firstPosition;
  uint16_t secondPosition;
};

void moveServoSilent(uint8_t servoId, uint16_t targetPos);
void initServoControlPins();
void moveServoWithControlPins();
void setFirstPosition(uint8_t servoId, uint16_t pos);
void setSecondPosition(uint8_t servoId, uint16_t pos);
uint16_t getFirstPosition(uint8_t servoId);
uint16_t getSecondPosition(uint8_t servoId);