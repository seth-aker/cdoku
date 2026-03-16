#ifndef SRC_SOLVER_UTILS_H
#define SRC_SOLVER_UTILS_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>   // For POSIX systems, use <io.h> for Windows
#include <stdio.h>
#include <errno.h> 

static bool debugModeOn = false;

static inline int randSudokuNum() {
  return (rand() % 9 + 1);
}

static inline bool includes(int* array, int arrayLen, int value) {
  for (int i = 0; i < arrayLen; ++i) {
    if (array[i] == value) {
      return true;
    }
  }
  return false;
}
static inline int maskToIntArray(uint16_t mask, int* array) {
  int count = 0;
  while (mask > 0) {
    int candidateIndex = __builtin_ctz(mask);
    array[count++] = candidateIndex;
    // Removes the lowest set bit.
    mask &= (mask - 1);
  }
  return count;
}

static inline void shuffleIntArr(int* array, int arrLen) {
  int temp;
  for (int i = arrLen - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

static inline bool doesDirExists(const char* path) {
  struct stat s;
  int err = stat(path, &s);
  if (err == -1) {
    if (ENOENT == errno) {
      return false;
    }
    else {
      perror('stat');
      exit(1);
    }
  }
  else {
    if (S_ISDIR(s.st_mode)) {
      return true;
    }
    else {
      return false;
    }
  }

}
#endif // SRC_SOLVER_UTILS_H
