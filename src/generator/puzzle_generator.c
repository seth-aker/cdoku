#include "puzzle_generator.h"
#include "puzzle.h"
#include "utils.h"

bool fillPuzzleRandomly(int cells[]) {
  int emptyIdx = findEmptyCell(cells);
  if (emptyIdx == -1) {
    return true;
  }

  int potentialVals[PUZZLE_WIDTH] = { 1,2,3,4,5,6,7,8,9 };
  shuffleIntArr(potentialVals, PUZZLE_WIDTH);
  for (int i = 0; i < PUZZLE_WIDTH; ++i) {

  }
}
