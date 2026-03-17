#include <stdlib.h>
#include <errno.h>
#include "log.h"
#include "puzzle.h"

int parsePuzzleStr(char* puzzleStr[], Puzzle* puzzle) {
  char* endptr;
  errno = 0;
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    char* str = &puzzleStr[i];
    long num = strtol(str, endptr, 10);
    if (endptr == str || *endptr != '\0') {
      log_error("Invalid Puzzle String! %s", str);
      return 1;
    }
    else if (errno == ERANGE) {
      log_error("Number out of range! %s", str);
      return 1;
    }
    else if (num < 0 || num > 9) {
      log_error("Number invalid: %ld", num);
      return 1;
    }
    puzzle->cells[i] = (int)num;
  }
  return 0;
}
