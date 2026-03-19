#include <servomotor.h>

static ServoControl servos[NUMBER_OF_SERVOS];

void allocateServoTimers() {
    // Maximum 16 timers on ESP32
    const uint8_t maxTimers = 16;

    // Allocate one timer per servo, up to the maximum allowed
    for (uint8_t i = 0; i < NUMBER_OF_SERVOS && i < maxTimers; i++) {
        ESP32PWM::allocateTimer(i);
    }
}

// Init contorl pins
void initServoControlPins(){
  int controlPins[NUMBER_OF_SERVOS] = {CONTROL_PIN_SGN_0, CONTROL_PIN_SGN_1, CONTROL_PIN_SGN_2, CONTROL_PIN_SGN_3};
  int outputPins[NUMBER_OF_SERVOS]  = {SERVO_OUTPUT_PIN_0, SERVO_OUTPUT_PIN_1, SERVO_OUTPUT_PIN_2, SERVO_OUTPUT_PIN_3};

  allocateServoTimers();

  for(int i = 0; i < NUMBER_OF_SERVOS; i++){
      servos[i].controlPin = controlPins[i];
      servos[i].servoOutputPin = outputPins[i];
      pinMode(servos[i].controlPin, INPUT);
      servos[i].servoHndl.setPeriodHertz(WORKING_FRERQUENCY);
  }
}

// The servo makes a lot of noises to try to keep
// the position. I just deattach it, because optical components
// do not require that function
void moveServoSilent(uint8_t servoId, uint16_t targetPos){
  uint16_t safePos = constrain(targetPos, PULSE_MIN, PULSE_MAX);
  servos[servoId].servoHndl.attach(servos[servoId].servoOutputPin, PULSE_MIN, PULSE_MAX);
  delay(100);
  servos[servoId].servoHndl.writeMicroseconds(safePos);
  delay(100);
  servos[servoId].servoHndl.detach();
}

// Read the values of the
// control pins and move if neccessary
// Not fast response, but not important for our application
void moveServoWithControlPins(){
  int currentRead;
  for(uint8_t servoId = 0; servoId < NUMBER_OF_SERVOS; servoId++){
    currentRead = digitalRead(servos[servoId].controlPin);
    // If the control signal has changed...
    if(currentRead != servos[servoId].controlSignalValue){
      // ... move the servos accordingly
      if(currentRead){
        moveServoSilent(servoId,servos[servoId].firstPosition);
      }else{
        moveServoSilent(servoId,servos[servoId].secondPosition);
      }
      // Update the value
      servos[servoId].controlSignalValue = currentRead;
    }
  }
}

// Functions to set/get position from outside this class
void setFirstPosition(uint8_t servoId, uint16_t pos){
  servos[servoId].firstPosition = pos;
}
void setSecondPosition(uint8_t servoId, uint16_t pos){
  servos[servoId].secondPosition = pos;
}

uint16_t getFirstPosition(uint8_t servoId){
  return servos[servoId].firstPosition;
}
uint16_t getSecondPosition(uint8_t servoId){
  return servos[servoId].secondPosition;
}