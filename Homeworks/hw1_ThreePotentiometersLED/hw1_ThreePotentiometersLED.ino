#define POT_R A0
#define POT_G A1
#define POT_B A2
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

void setup() {
  Serial.begin(9600);
  pinMode(POT_R, INPUT);
  pinMode(POT_G, INPUT);
  pinMode(POT_B, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void loop() {
  int sensorValueR = analogRead(POT_R);
  int sensorValueG = analogRead(POT_G);
  int sensorValueB = analogRead(POT_B);
  int redVal = map(sensorValueR, 0, 1023, 0, 255);
  int greenVal = map(sensorValueG, 0, 1023, 0, 255);
  int blueVal = map(sensorValueB, 0, 1023, 0, 255);
  setColor(redVal, greenVal, blueVal);
}
