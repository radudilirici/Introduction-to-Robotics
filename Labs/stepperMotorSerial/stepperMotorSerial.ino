/* Example sketch to control a 28BYJ-48 stepper motor with ULN2003 driver board and Arduino UNO. More info: https://www.makerguides.com */
// Include the Arduino Stepper.h library:
#include <Stepper.h>
// Define number of steps per rotation:
const int stepsPerRevolution = 2048;
// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver
// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
int stepperPosition = 0;

int angleValue = 0;
int angleDir = 1;
int angleSteps = 0;

void changeStepperAngle() {
  if (angleSteps != 0) {
    myStepper.step(angleSteps);
    angleSteps = 0;
  }
}

void readSerialAngle() {
  if (Serial.available()) {
    angleValue = Serial.parseInt();
    angleDir = Serial.parseInt();
    char aux = Serial.read();  // enter
    angleSteps = map(angleValue, 0, 360, 0, stepsPerRevolution * angleDir);
    Serial.println(angleSteps);
  }
}

void setup() {
  // Set the speed to 8 rpm:
  myStepper.setSpeed(8);

  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);
}

void loop() {
  readSerialAngle();
  changeStepperAngle();
}
