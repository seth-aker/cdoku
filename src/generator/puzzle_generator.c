#include "puzzle_generator.h"
#include "puzzle.h"
#include "utils.h"
#include "alg_x_solve.h"

void generatePuzzle(DiffRating targetDiff, uint8_t cells[], int* diffScore) {
  
}

bool fillPuzzleRandomly(uint8_t cells[]) {
  int emptyIdx = findEmptyCell(cells);
  if (emptyIdx == -1) {
    return true;
  }

  uint8_t potentialVals[PUZZLE_WIDTH] = { 1,2,3,4,5,6,7,8,9 };
  shuffleIntArr(potentialVals, PUZZLE_WIDTH);
  for (int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(isValidNumInCell(potentialVals[i], emptyIdx, cells)) {
      cells[emptyIdx] = potentialVals[i];

      if(fillPuzzleRandomly(cells)) {
        return true;
      }
      cells[emptyIdx] = 0;
    }
  }
  return false;
}

// removes a random value and then checks for a unique solution.
bool removeRandomVal(uint8_t cells[]) {
  uint8_t filledCellsIdx[TOTAL_CELLS] = { -1 };
  int filledCount = 0;
  bool removed = false;
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    if (cells[i] != 0) {
      filledCellsIdx[filledCount] = i;
      filledCount++;
    }
  }
  if(filledCount < 17) { // the absolute mathematically smallest number of filled cells for sudoku
    return false;
  }

  shuffle_uint_8_array(filledCellsIdx, filledCount);
  for(int i = 0; i < filledCount; ++i) {
    uint8_t backtrackVal = cells[filledCellsIdx[i]];
    cells[filledCellsIdx[i]] = 0;
    if(!algorithm_x_has_unique_sol(cells)) {
      cells[filledCellsIdx[i]] = backtrackVal;
    } else {
      return true;
    }
  }
}