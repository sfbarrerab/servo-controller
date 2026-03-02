#include <servomotor.h> // servo functions 

// The servo makes a lot of noises to try to keep
// the position. I just deattach it, because optical components
// do not require that function

void moveServoSilent(Servo myservo, int servoPin, int minPos, int maxPos, int targetPos){
  myservo.attach(servoPin, minPos, maxPos);
  delay(100);
  myservo.writeMicroseconds(targetPos);
  delay(100);
  myservo.detach();
}