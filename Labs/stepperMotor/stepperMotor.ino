const int stepperPin1 = 8;
const int stepperPin2 = 9;
const int stepperPin3 = 10;
const int stepperPin4 = 11;

int delayTime = 50;

void setup() {
  pinMode(stepperPin1, OUTPUT);
  pinMode(stepperPin2, OUTPUT);
  pinMode(stepperPin3, OUTPUT);
  pinMode(stepperPin4, OUTPUT);
}

void loop() {
  digitalWrite(stepperPin1, HIGH);
  digitalWrite(stepperPin2, LOW);
  digitalWrite(stepperPin3, LOW);
  digitalWrite(stepperPin4, LOW);
  delay(delayTime);
  
  digitalWrite(stepperPin1, LOW);
  digitalWrite(stepperPin2, HIGH);
  digitalWrite(stepperPin3, LOW);
  digitalWrite(stepperPin4, LOW);
  delay(delayTime);
  
  digitalWrite(stepperPin1, LOW);
  digitalWrite(stepperPin2, LOW);
  digitalWrite(stepperPin3, HIGH);
  digitalWrite(stepperPin4, LOW);
  delay(delayTime);
 
  digitalWrite(stepperPin1, LOW);
  digitalWrite(stepperPin2, LOW);
  digitalWrite(stepperPin3, LOW);
  digitalWrite(stepperPin4, HIGH);
  delay(delayTime);

}
