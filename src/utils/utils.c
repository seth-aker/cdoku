#include "utils.h"
#include <stdlib.h>

void shuffleIntArray(int arr[], int arrLen) {
  int temp;
  for (int i = arrLen - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}
