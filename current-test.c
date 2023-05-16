int printf();

int main() {
  int i;
  int j;
  int k;
  int l;
  for (i = 0; i < 5; i = i + 1) {
    for (j = 0; j < 4; j = j + 1) {
      printf("hi there %d %d\n", i, j);
      if (j > 3) {
        break;
      }
    }
  }
  for (k = 0; k < 5; k = k + 1) {
    printf("hi");
  }
  return 0;
}

