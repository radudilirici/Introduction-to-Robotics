#include <Servo.h>
 
int servoPin = 9;
Servo servo; 
 
int angle = 0;   // servo position in degrees

const int potPin = A0;
 
void setup()
{
  servo.attach(servoPin);
  pinMode(potPin, INPUT);
}
 
 
void loop()
{
  updateAngle();                               
  servo.write(angle);
}

void updateAngle() {
  angle = map(analogRead(potPin), 0, 1023, 0, 180);
}
