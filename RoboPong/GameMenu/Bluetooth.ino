void BTSetup() {
  BTSerial.begin(9600);
}

int BTRead() {
  if (BTSerial.available()) {
    int a = BTSerial.read();
    return a;
  }
}
