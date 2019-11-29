int incomingByte = 0;

void setup() {
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available() > 0) {
    incomingByte = Serial.read();/*
    Serial.print("I recived(DEC): ");
    Serial.println(incomingByte, DEC);
    Serial.print("I recived(HEX): ");
    Serial.println(incomingByte, HEX);
    Serial.print("I recived(OCT): ");
    Serial.println(incomingByte, OCT);
    Serial.print("I recived(BIN): ");
    Serial.println(incomingByte, BIN);*/
    char ch = (char)incomingByte;
//    Serial.print("I recived: ");
    if (incomingByte == 10) {
      Serial.println();
    }
    else {
      Serial.print(ch);
    }
  }
}
