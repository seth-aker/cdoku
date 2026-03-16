#include <stdlib.h>
#include <errno.h>

#include "puzzle.h"

int parsePuzzleStr(char* puzzleStr[], Puzzle* puzzle) {
  char* endptr;

  for(int i = 0; i < TOTAL_CELLS; ++i) {
    char* str = &puzzleStr[i];
    int num = strtol(str, endptr, 10);
    if(endptr == str) {
      
    } 
  }
}