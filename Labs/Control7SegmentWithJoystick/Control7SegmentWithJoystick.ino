const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;

const int segSize = 8;
const int noOfDigits = 10;

int index = 0;
int state = HIGH;
int dpState = LOW;

const int pinX = A0;
const int pinY = A1;
const int buttonPin = 10;
const int millisToWait = 300;

const int highX = 800;
const int lowX = 200;

int xAxis;
int yAxis;
int buttonState;
int lastButtonState = HIGH;

unsigned long int lastMillis = 0;

int segments[] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for(int i = 0; i < segSize - 1; ++i) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], decimalPoint);
}

void setup() {
//  pinMode(pinX, INPUT);
//  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  xAxis = analogRead(pinX);
  yAxis = analogRead(pinY);
  buttonState = digitalRead(buttonPin);
  Serial.print(xAxis);
  Serial.print("\t");
  Serial.print(yAxis);
  Serial.print("\t");
  Serial.print(buttonState);
  Serial.print("\t");
  Serial.print(lastButtonState);
  Serial.print("\n");
  if (xAxis >= highX && millis() - lastMillis > millisToWait) {
    ++ index;
    if (index == noOfDigits) {
      index = 0;
    }
    lastMillis = millis();
  }
  if (xAxis <= lowX && millis() - lastMillis > millisToWait) {
    -- index;
    if (index < 0) {
      index = noOfDigits - 1;
    }
    lastMillis = millis();
  }
  if (buttonState != lastButtonState){
    if (buttonState == LOW) {
      dpState = !dpState;
    }
    lastButtonState = buttonState;
  }
  displayNumber(index, dpState);
}
