const int row[] = {6, 11, A5, 9, 5, A4, 4, A2};
                 //C1 C2 C3 C4  C5 C6  C7  C8 - anodes
const int col[] = {10, 3, 2, 7, A3, 8, 12, 13};

const int displaySize = 8;
const int matrixRows = 8;
const int matrixCols = 16;

unsigned long int lastMillis = 0;
const int millisDelay = 100;

int offset = 0;

int matrix[matrixRows][matrixCols] = {
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0,  0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0,  0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0,  0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
};

void print_matrix(int matrix[matrixRows][matrixCols], int colOffset){
  for (int i = 0; i < displaySize; i++){
    for (int nrCol = 0; nrCol < displaySize; nrCol++){
      int j = (nrCol + colOffset) % matrixCols;
      digitalWrite(col[nrCol], matrix[i][j]);
    }
    digitalWrite(row[i], LOW);
    delay(2);
    digitalWrite(row[i], HIGH);
  }
}

void setup() {
  for (int i = 0; i < displaySize; i++){
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  for (int i = 0; i < displaySize; i++){
    digitalWrite(col[i], LOW);
    digitalWrite(row[i], HIGH);
  }
}

void loop() {
  if (millis() - lastMillis > millisDelay){
    offset = (offset + 1) % matrixCols;
    lastMillis = millis();
  }
  print_matrix(matrix, offset);
}
