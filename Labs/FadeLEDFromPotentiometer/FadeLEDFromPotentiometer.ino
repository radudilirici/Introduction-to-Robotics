/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
*/

const int ledPin = 9;
const int potPin = A0;
int brightness;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  int potVal = analogRead(potPin);
//  Serial.println(potVal);
  brightness = map(potVal, 0, 1023, 0, 255);
//  Serial.println(brightness);
  analogWrite(ledPin, brightness);
  float voltage = potVal * (5.0 / 1023.0);
  Serial.println(voltage);
}
