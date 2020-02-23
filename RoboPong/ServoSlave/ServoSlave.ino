#include <Wire.h>
#include <Servo.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9

const int servoPin = 9;
Servo servo;

byte baseAngle = 100;  // the functions will use angles relative to this position
int rightAngle = -25;
int leftAngle = 25;

int servoRead() {
  return servo.read() - baseAngle;
}

void servoWrite(int angle) {  // set the angle of the servo relative to the default position, which is not at 0 degrees
  if (servoRead() - angle == 0) {
    return;
  }
  if (baseAngle + angle < 0 or baseAngle + angle > 180) {
    return;
  }
  servo.write(baseAngle + angle);
}

bool servoMoveDir(int xDir) {
  int angle;
  if (xDir == 0) {
    angle = 0;
  }
  else if (xDir == 1) {
    angle = rightAngle;
  }
  else if (xDir == -1) {
    angle = leftAngle;
  }
  else {
    return false;
  }
  servoWrite(angle);
  return true;
}

void setup() {
  servo.attach(servoPin);

  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
  
  // Function to run when data received from master
  Wire.onReceive(receiveEvent);

  servo.write(baseAngle);
}

void receiveEvent() {
  // Read while data received
  while (Wire.available() > 0) {
    int dir = Wire.read() - 1;  // the recieved data cannot be negative so it's sent with +1
    servoMoveDir(dir);
  }
}

void loop() {
  //  the slave board will check for events without putting something here
}
