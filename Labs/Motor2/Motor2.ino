int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
const int potPin = A0;
const int buttonPin = 7;

int motorSpeed = 255;
boolean reverse = LOW;

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP 
  );
}
 
void loop()
{
  updateMotorSpeed();
  updateDirection();
  setMotor(motorSpeed , reverse);
}

void updateMotorSpeed() {
  motorSpeed = map(analogRead(potPin), 0, 1023, 0, 255);
}

void updateDirection() {
  reverse = digitalRead(buttonPin);
}
 
void setMotor(int motorSpeed , boolean reverse)
{
  analogWrite(enablePin, motorSpeed);
  digitalWrite(in1Pin, !reverse);
  digitalWrite(in2Pin, reverse);
}
