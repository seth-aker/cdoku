#ifndef SRC_SOLVER_UTILS_H
#define SRC_SOLVER_UTILS_H

#include <stdbool.h>

static bool debugModeOn = true;

static inline bool includes(int* array, int arrayLen, int value) {
  for(int i = 0; i < arrayLen; ++i) {
    if(array[i] == value) {
      return true;
    }
  }
  return false;
}
static inline int maskToIntArray(uint16_t mask, int* array) {
  int count = 0;
  while(mask > 0) {
    int candidateIndex = __builtin_ctz(mask);
    array[count++] = candidateIndex;
    // Removes the lowest set bit.
    mask &= (mask - 1);
  }
  return count;
}
#endif // SRC_SOLVER_UTILS_H