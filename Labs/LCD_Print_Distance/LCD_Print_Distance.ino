#include <LiquidCrystal.h>

const int RSPin = 12;
const int EPin = 11;
const int D4Pin = 5;
const int D5Pin = 4;
const int D6Pin = 3;
const int D7Pin = 2;

LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);

const int trigPin = 9;
const int echoPin = 10;

const int millisDelay = 500;
unsigned long int lastMillis = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Distance:");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
//  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  if (millis() - lastMillis > millisDelay) {
    lcd.clear();
//    lcd.setCursor(0, 1);
//    lcd.print("                ");
    lcd.print("Distance:");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lastMillis = millis();
  }
}
