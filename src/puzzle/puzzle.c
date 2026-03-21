#include <stdlib.h>
#include <errno.h>
#include "log.h"
#include "puzzle.h"

int findEmptyCell(uint8_t cells[]) {
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    if (cells[i] == 0) {
      return i;
    }
  }
  return -1;
}

int parsePuzzleStr(char puzzle_str[], Puzzle* puzzle) {
  char* endptr;
  errno = 0;
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    char* str = &puzzle_str[i];
    long num = strtol(str, &endptr, 10);
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

bool isValidNumInCell(uint8_t num, int idx, uint8_t cells[]) {
  const uint8_t* peers = CELL_PEERS_LOOKUP[idx];
  for (int i = 0; i < 20; ++i) {
    int peer_idx = peers[i];
    if (cells[peer_idx] == num) {
      return false;
    }
  }
  return true;
}
