const int row[] = {6, 11, A5, 9, 5, A4, 4, A2};
const int col[] = {10, 3, 2, 7, A3, 8, 12, 13};

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++){
    digitalWrite(col[i], LOW);
    digitalWrite(row[i], HIGH);
  }
//  digitalWrite(col[0], HIGH);
//  digitalWrite(row[0], LOW);
}

int r, c;

void loop() {
  for (r = 0; ; r = (r + 1) % 8){
  digitalWrite(row[r], LOW);
  for (c = 0; c < 8; c++){
    digitalWrite(col[c], HIGH);
    delay(100);
    digitalWrite(col[c], LOW);
  }
  digitalWrite(row[r], HIGH);
  }
}
