#ifndef SRC_SOLVER_UTILS_H
#define SRC_SOLVER_UTILS_H

#include <stdbool.h>
#include <stdio.h>

static inline bool includes(int* array, int arrayLen, int value) {
  for(int i = 0; i < arrayLen; ++i) {
    if(array[i] == value) {
      return true;
    }
  }
  return false;
}
static inline int factorial(int num) {
  if(num < 0) {
    printf("Error, cannot factorial negative number: %d\n", num);
    return -1;
  }

  int fact = 1;
  for(int i = num; i > 1; --i) {
    fact *= i;
  }
  return fact;
}

#endif // SRC_SOLVER_UTILS_H