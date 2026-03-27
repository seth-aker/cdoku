#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

void shuffle_uint_8_array(uint8_t arr[], int arrLen) {
  uint8_t temp;
  for (int i = arrLen - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

bool includes_int(int array[], int arrLen, int num) {
  for (int i = 0; i < arrLen; ++i) {
    if (array[i] == num) {
      return true;
    }
  }
  return false;
}
