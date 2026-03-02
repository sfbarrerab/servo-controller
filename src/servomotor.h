
#include <Servo.h>  // Arduino library
#include <Arduino.h>

#define NUMBER_OF_SERVOS 4

// the pulse width goes from 450 to 1050 microseconds
// being 750 microseconds the center position
#define PULSE_MIN   450
#define PULSE_MAX   1050
#define PULSE_CENTER 750

// define output pins of the servos PWM signal
#define SERVO_OUTPUT_PIN_0 9

// define addresses where to save the positions
#define ADDR_FIRST_POS_ND_SERVO 120
#define ADDR_SECOND_POS_ND_SERVO 121
#define ADDR_FIRST_POS_PM_SERVO 122
#define ADDR_SECOND_POS_PM_SERVO 123

struct ServoControl {
  Servo servoHndl;
  int controlPin;
  int servoPin;
  int minPosition;
  int maxPosition;
};

typedef enum
{
    SERVO_ND,
    SERVO_PM,
    SERVO_2,
    SERVO_3
} ServoId;

void moveServoSilent(int servoId, int targetPos);