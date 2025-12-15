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

#endif // SRC_SOLVER_UTILS_H