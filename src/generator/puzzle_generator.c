#include <stdbool.h>
#include <string.h>
#include "puzzle.h"
#include "puzzle_generator.h"
#include "alg_x_solve.h"
#include "logical_solve.h"
#include "utils.h"

void generate_puzzle(Puzzle* puzzle, DiffRating target_difficulty) {
  fill_puzzle_randomly(puzzle->cells);
  Puzzle puzzle_cpy;
  while(true) {
    bool is_still_unique = remove_random_val(puzzle->cells);
    if(!is_still_unique) {
      // reset and try again
      reset_puzzle(puzzle);
      continue;
    }
    clone_puzzle(&puzzle_cpy, puzzle);
    solve_puzzle(&puzzle_cpy);

    if(puzzle_cpy.difficulty.rating < target_difficulty) {
      continue;
    }
    if(puzzle_cpy.difficulty.rating == target_difficulty) {
      puzzle->difficulty = puzzle_cpy.difficulty;
      return;
    }
    if(puzzle_cpy.difficulty.rating > target_difficulty) {
      reset_puzzle(puzzle);
    }
  }
}
bool fill_puzzle_randomly(uint8_t cells[]) {
  int empty_idx = find_empty_cell(cells);
  if(empty_idx == -1) {
    return true;
  }

  uint8_t potential_vals[PUZZLE_WIDTH] = { 1,2,3,4,5,6,7,8,9 };
  shuffle_uint_8_array(potential_vals, PUZZLE_WIDTH);
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(is_valid_num_in_cell(potential_vals[i], empty_idx, cells)) {
      cells[empty_idx] = potential_vals[i];

      if(fill_puzzle_randomly(cells)) {
        return true;
      }
      cells[empty_idx] = 0;
    }
  }
  return false;
}

// removes a random value and then checks for a unique solution.
bool remove_random_val(uint8_t cells[]) {
  uint8_t filled_cells_idx[TOTAL_CELLS] = { -1 };
  int filled_count = 0;
  bool removed = false;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(cells[i] != 0) {
      filled_cells_idx[filled_count] = i;
      filled_count++;
    }
  }
  if(filled_count < 17) { // the absolute mathematically smallest number of filled cells for sudoku
    return false;
  }

  shuffle_uint_8_array(filled_cells_idx, filled_count);
  for(int i = 0; i < filled_count; ++i) {
    uint8_t backtrack_val = cells[filled_cells_idx[i]];
    cells[filled_cells_idx[i]] = 0;
    if(!algorithm_x_has_unique_sol(cells)) {
      cells[filled_cells_idx[i]] = backtrack_val;
    } else {
      return true;
    }
  }
  return false;
}

void reset_puzzle(Puzzle* puzzle) {
  memset(puzzle, 0, sizeof(Puzzle));
  fill_puzzle_randomly(puzzle->cells);
}
