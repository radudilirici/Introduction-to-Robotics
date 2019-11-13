const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int segSize = 8;
const int noOfDigits = 10;
const int noOfDisplays = 4;

const int pinX = A0;
const int pinY = A1;
const int buttonPin = A2;
const int millisToWait = 300;
const int millisForBlink = 300;

const int highAxis = 800;
const int lowAxis = 200;

int xAxis;
int yAxis;
int buttonState;
int lastButtonState = HIGH;
int dpState = LOW;
int digitNo = 3;
//int noToShow = 0;
int digitSelected = 0;
int blinkStatus = LOW;

unsigned long int lastMillis = 0;
unsigned long int lastBlink = 0;

int segments[] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int digits[] = {
  pinD1, pinD2, pinD3, pinD4
};
int numbersToShow[] = {
  0, 0, 0, 0
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
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[noOfDisplays - num - 1], LOW);
}

void displayAllNumbers(int numbers[]){
   for (int i = 0; i < noOfDisplays; i++) {
    showDigit(i);
    if (digitSelected == i) {
      if (millis() - lastBlink >= millisForBlink) {
        blinkStatus = !blinkStatus;
        lastBlink = millis();
      }
      displayNumber(numbers[i], blinkStatus);
    }
    else if (digitSelected == -i - 1){
      displayNumber(numbers[i], HIGH);
    }
    else {
      displayNumber(numbers[i], LOW);
    }
    delay(2);
  }
}

void setup() {
//  pinMode(pinX, INPUT);
//  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);  
  }
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);  
  }
  Serial.begin(9600);
}

void loop() {
  xAxis = analogRead(pinX);
  yAxis = analogRead(pinY);
  buttonState = digitalRead(buttonPin);
  Serial.println(blinkStatus);/*
  Serial.println(digitSelected);
  for (int i = 0; i < 4; i++){
    Serial.print(numbersToShow[i]);
  }
  Serial.print("\n");
  Serial.print(xAxis);
  Serial.print("\t");
  Serial.print(yAxis);
  Serial.print("\t");
  Serial.print(buttonState);
  Serial.print("\t");
  Serial.print(lastButtonState);
  Serial.print("\n");*/
  if (digitSelected >= 0) {
    if (yAxis >= highAxis && millis() - lastMillis > millisToWait) {
      -- numbersToShow[digitSelected];
      if (numbersToShow[digitSelected] < 0) {
        numbersToShow[digitSelected] = 9;
      }
      lastMillis = millis();
    }
    if (yAxis <= lowAxis && millis() - lastMillis > millisToWait) {
      ++ numbersToShow[digitSelected];
      if (numbersToShow[digitSelected] == noOfDigits) {
        numbersToShow[digitSelected] = 0;
      }
      lastMillis = millis();
    }
    if (buttonState != lastButtonState){
      if (buttonState == LOW) {
        dpState = !dpState;
        digitSelected = -digitSelected - 1;
      }
      lastButtonState = buttonState;
    }
  }
  else {
    if (xAxis >= highAxis && millis() - lastMillis > millisToWait) {
      -- digitSelected;
      if (digitSelected == -noOfDisplays - 1) {
        digitSelected = -1;
      }
      lastMillis = millis();
    }
    if (xAxis <= lowAxis && millis() - lastMillis > millisToWait) {
      ++ digitSelected;
      if (digitSelected == 0) {
        digitSelected = -noOfDisplays;
      }
      lastMillis = millis();
    }
    if (buttonState != lastButtonState){
      if (buttonState == LOW) {
        dpState = !dpState;
        digitSelected = -digitSelected - 1;
      }
      lastButtonState = buttonState;
    }
  }
  displayAllNumbers(numbersToShow);
}
