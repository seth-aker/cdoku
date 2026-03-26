#include <stdbool.h>
#include <string.h>
#include "puzzle_generator.h"
#include "puzzle.h"
#include "utils.h"
#include "alg_x_solve.h"

void generate_puzzle(DiffRating targetDiff, uint8_t cells[], int* diffScore) {
  bool isAtDiff = false;
  fill_puzzle_randomly(cells);
  while (!isAtDiff) {
    bool isStillUnique = remove_random_val(cells);
    if (!isStillUnique) {
      // reset and try again
      reset_puzzle(cells);
      continue;
    }
    // *diffScore = determineDiff(cells);
  }
}

bool fill_puzzle_randomly(uint8_t cells[]) {
  int emptyIdx = find_empty_cell(cells);
  if (emptyIdx == -1) {
    return true;
  }

  uint8_t potentialVals[PUZZLE_WIDTH] = { 1,2,3,4,5,6,7,8,9 };
  shuffle_uint_8_array(potentialVals, PUZZLE_WIDTH);
  for (int i = 0; i < PUZZLE_WIDTH; ++i) {
    if (is_valid_num_in_cell(potentialVals[i], emptyIdx, cells)) {
      cells[emptyIdx] = potentialVals[i];

      if (fill_puzzle_randomly(cells)) {
        return true;
      }
      cells[emptyIdx] = 0;
    }
  }
  return false;
}

// removes a random value and then checks for a unique solution.
bool remove_random_val(uint8_t cells[]) {
  uint8_t filledCellsIdx[TOTAL_CELLS] = { -1 };
  int filledCount = 0;
  bool removed = false;
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    if (cells[i] != 0) {
      filledCellsIdx[filledCount] = i;
      filledCount++;
    }
  }
  if (filledCount < 17) { // the absolute mathematically smallest number of filled cells for sudoku
    return false;
  }

  shuffle_uint_8_array(filledCellsIdx, filledCount);
  for (int i = 0; i < filledCount; ++i) {
    uint8_t backtrackVal = cells[filledCellsIdx[i]];
    cells[filledCellsIdx[i]] = 0;
    if (!algorithm_x_has_unique_sol(cells)) {
      cells[filledCellsIdx[i]] = backtrackVal;
    }
    else {
      return true;
    }
  }
}

void reset_puzzle(uint8_t cells[]) {
  memset(cells, 0, sizeof(uint8_t) * TOTAL_CELLS);
  fill_puzzle_randomly(cells);
}
