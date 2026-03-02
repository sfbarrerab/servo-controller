#include <servomotor.h>

static ServoControl servos[NUMBER_OF_SERVOS];

// Init contorl pins
void initServoControlPins(){
  int controlPins[NUMBER_OF_SERVOS] = {CONTROL_PIN_SGN_0, CONTROL_PIN_SGN_1, CONTROL_PIN_SGN_2, CONTROL_PIN_SGN_3};
  int outputPins[NUMBER_OF_SERVOS]  = {SERVO_OUTPUT_PIN_0, SERVO_OUTPUT_PIN_1, SERVO_OUTPUT_PIN_2, SERVO_OUTPUT_PIN_3};

  for(int i = 0; i < NUMBER_OF_SERVOS; i++){
      servos[i].controlPin = controlPins[i];
      servos[i].servoOutputPin = outputPins[i];
      pinMode(servos[i].controlPin, INPUT);
  }
}

// The servo makes a lot of noises to try to keep
// the position. I just deattach it, because optical components
// do not require that function
void moveServoSilent(int servoId, int targetPos){
  servos[servoId].servoHndl.attach(servos[servoId].servoOutputPin, PULSE_MIN, PULSE_MAX);
  delay(30);
  // int safePos = constrain(targetPos, PULSE_MIN, PULSE_MAX);
  servos[servoId].servoHndl.writeMicroseconds(targetPos);
  delay(30);
  servos[servoId].servoHndl.detach();
}

// Read the values of the
// control pins and move if neccessary
// Not fast response, but not important for our application
void moveServoWithControlPins(){
  int currentRead;
  for(int servoId = 0; servoId < NUMBER_OF_SERVOS; servoId++){
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
void setFirstPosition(int servoId, int pos){
  servos[servoId].firstPosition = pos;
}
void setSecondPosition(int servoId, int pos){
  servos[servoId].secondPosition = pos;
}

int getFirstPosition(int servoId){
  return servos[servoId].firstPosition;
}
int getSecondPosition(int servoId){
  return servos[servoId].secondPosition;
}