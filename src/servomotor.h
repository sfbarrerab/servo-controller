#pragma once
#include <Servo.h>  // Arduino library
#include <Arduino.h>

#define NUMBER_OF_SERVOS 4

// the pulse width goes from 450 to 1050 microseconds
// being 750 microseconds the center position
#define PULSE_MIN   450
#define PULSE_MAX   1050
#define PULSE_CENTER 750

// define output pins of the servos PWM signal
#define SERVO_OUTPUT_PIN_0 3
#define SERVO_OUTPUT_PIN_1 5
#define SERVO_OUTPUT_PIN_2 6
#define SERVO_OUTPUT_PIN_3 9

// Pins to control the servo triggering to change position
#define CONTROL_PIN_SGN_0 2
#define CONTROL_PIN_SGN_1 4
#define CONTROL_PIN_SGN_2 7
#define CONTROL_PIN_SGN_3 8

struct ServoControl {
  Servo servoHndl;
  int controlPin;
  int controlSignalValue = 0;
  int servoOutputPin;
  int firstPosition;
  int secondPosition;
};

void moveServoSilent(int servoId, int targetPos);
void initServoControlPins();
void moveServoWithControlPins();
void setFirstPosition(int servoId, int pos);
void setSecondPosition(int servoId, int pos);
int getFirstPosition(int servoId);
int getSecondPosition(int servoId);