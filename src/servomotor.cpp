#include <servomotor.h> // servo functions 

static ServoControl servos[NUMBER_OF_SERVOS];

// The servo makes a lot of noises to try to keep
// the position. I just deattach it, because optical components
// do not require that function
void moveServoSilent(int servoId, int targetPos){
  servos[servoId].servoHndl.attach(servos[servoId].servoPin, PULSE_MIN, PULSE_MAX);
  delay(100);
  servos[servoId].servoHndl.writeMicroseconds(targetPos);
  delay(100);
  servos[servoId].servoHndl.detach();
}