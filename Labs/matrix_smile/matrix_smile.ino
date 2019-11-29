const int row[] = {6, 11, A5, 9, 5, A4, 4, A2};
                 //C1 C2 C3 C4  C5 C6  C7  C8 - anodes
const int col[] = {10, 3, 2, 7, A3, 8, 12, 13};

const int matrixSize = 8;

int matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

void print_matrix(int matrix[matrixSize][matrixSize]){
  for (int i = 0; i < matrixSize; i++){
    for (int j = 0; j < matrixSize; j++){
      digitalWrite(col[j], matrix[i][j]);
    }
    digitalWrite(row[i], LOW);
    delay(2);
    digitalWrite(row[i], HIGH);
  }
}

void setup() {
  for (int i = 0; i < matrixSize; i++){
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
  }
  for (int i = 0; i < matrixSize; i++){
    digitalWrite(col[i], LOW);
    digitalWrite(row[i], HIGH);
  }
}

void loop() {
  print_matrix(matrix);
}
