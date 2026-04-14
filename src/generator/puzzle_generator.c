#include <stdbool.h>
#include <string.h>
#include "puzzle.h"
#include "puzzle_generator.h"
#include "alg_x_solve.h"
#include "logical_solve.h"
#include "utils.h"
#include "log.h"

void generate_puzzle(Puzzle* puzzle, DiffRating target_difficulty) {
  fill_puzzle_randomly(puzzle->cells);
  #ifndef NDEBUG 
    char str_buff[110];
    stringify_puzzle(str_buff, 110, puzzle);
    log_debug("INITIAL STATE: %s", str_buff);
  #endif
  Puzzle puzzle_cpy;
  int min_cells_removed = define_min_cells_removed(target_difficulty);
  int cells_removed = 0;
  while(true) {
    bool is_still_unique = remove_random_val(puzzle);
    cells_removed++;

    // if(cells_removed < min_cells_removed) {
    //   continue;
    // }
    if(!is_still_unique) {
      // reset and try again
      reset_puzzle(puzzle);
      cells_removed = 0;
      continue;
    }
    clone_puzzle(&puzzle_cpy, puzzle);
    fill_puzzle_candidates(&puzzle_cpy);
    if(!solve_puzzle(&puzzle_cpy)) {
      reset_puzzle(puzzle);
      cells_removed = 0;
      continue;
    }

    if(puzzle_cpy.difficulty.rating < target_difficulty || cells_removed < min_cells_removed) {
      continue;
    }
    if(puzzle_cpy.difficulty.rating == target_difficulty) {
      puzzle->difficulty.hardest_step = puzzle_cpy.difficulty.hardest_step;
      puzzle->difficulty.rating = puzzle_cpy.difficulty.rating;
      puzzle->difficulty.score = puzzle_cpy.difficulty.score;
      memcpy(puzzle->solution, &puzzle_cpy.solution, sizeof(Step) * 729);
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
bool remove_random_val(Puzzle* puzzle) {
  uint8_t filled_cells_idx[TOTAL_CELLS];
  int filled_count = 0;
  bool removed = false;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(puzzle->cells[i] != 0) {
      filled_cells_idx[filled_count] = i;
      filled_count++;
    }
  }
  if(filled_count < 17) { // the absolute mathematically smallest number of filled cells for sudoku
    return false;
  }

  shuffle_uint_8_array(filled_cells_idx, filled_count);
  for(int i = 0; i < filled_count; ++i) {
    uint8_t backtrack_val = puzzle->cells[filled_cells_idx[i]];
    puzzle->cells[filled_cells_idx[i]] = 0;
    if(!algorithm_x_has_unique_sol(puzzle->cells)) {
      puzzle->cells[filled_cells_idx[i]] = backtrack_val;
    } else {
      log_debug("Value %d removed from cell index %d", backtrack_val, filled_cells_idx[i]);
      return true;
    }
  }
  return false;
}
int define_min_cells_removed(DiffRating target_difficulty) {
  switch(target_difficulty)
  {
  case BEGINNER:
    return 36 + rand() % 8; // remove 36 - 43 clues
  case EASY:
    return 44 + rand() % 6; // remove 44 - 49 clues
  case MEDIUM:
    return 50 + rand() % 5; // remove between 50 and 54 clues
  case HARD:
    return 55 + rand() % 5; // remove between 55 and 59 clues
  default:
    return 55 + rand() % 8; // remove between 55 and 62
  }
}
void reset_puzzle(Puzzle* puzzle) {
  memset(puzzle, 0, sizeof(Puzzle));
  fill_puzzle_randomly(puzzle->cells);
  #ifndef NDEBUG 
  char str_buff[110];
  stringify_puzzle(str_buff, 110, puzzle);
  log_debug("INITIAL STATE: %s", str_buff);
  #endif
}
