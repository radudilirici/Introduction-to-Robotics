int motorPin = 3;
int motorSpeed = 140;
int aux;

void setup()
{
 pinMode(motorPin, OUTPUT);
 Serial.begin(9600);
 while (!Serial);
 Serial.println("Speed 0 to 255");
}


void loop()
{
 if (motorSpeed  >= 0 && motorSpeed <= 255)
 {
   analogWrite(motorPin, motorSpeed);
 }
// Serial.println(motorSpeed);
 if (Serial.available() > 0)
 {
   motorSpeed = Serial.parseInt();
   aux = Serial.read();
 }
}
