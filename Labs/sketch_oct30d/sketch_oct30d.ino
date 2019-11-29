const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(redPin, 200);
  analogWrite(greenPin, 123);
  analogWrite(bluePin, 47);
}
