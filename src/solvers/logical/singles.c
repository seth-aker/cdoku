#include "singles.h"

#include <stdbool.h>
#include "puzzle.h"

bool find_naked_single(Puzzle* puzzle) {
  bool progress_made = false;
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    if (puzzle->cells[i] != 0) {
      continue;
    }
    if (__builtin_popcount(puzzle->candidates[i]) != 1) {
      continue;
    }
    bool is_full_house = check_is_full_house(puzzle, i);
    Step step = {
      .eliminated_mask = puzzle->candidates[i],
      .placed_val = (uint8_t)(puzzle->candidates[i]),
      .target_cell = (uint8_t)i,
      .technique = is_full_house ? FULL_HOUSE : NAKED_SINGLE
    };
    apply_step(puzzle, step);
    progress_made = true;
  }
  return progress_made;
}
bool check_is_full_house(Puzzle* puzzle, uint8_t idx) {
  bool is_full_house_row = true;
  bool is_full_house_col = true;
  bool is_full_house_block = true;

  uint8_t row = IDX_TO_ROW[idx];
  House house;
  get_row(row, puzzle, &house);
  uint8_t empty_cell_count = 0;
  for (int k = 0; k < PUZZLE_WIDTH; ++k) {
    if (house.cells[k] == 0) {
      empty_cell_count++;
    }
    if (empty_cell_count > 1) {
      is_full_house_row = false;
      break;
    }
  }
  if (is_full_house_row) {
    return true;
  }
  uint8_t col = IDX_TO_COL[idx];
  get_col(col, puzzle, &house);
  empty_cell_count = 0;
  for (int k = 0; k < PUZZLE_WIDTH; ++k) {
    if (house.cells[k] == 0) {
      empty_cell_count++;
    }
    if (empty_cell_count > 1) {
      is_full_house_col = false;
      break;
    }
  }
  if (is_full_house_col) {
    return true;
  }

  uint8_t block = IDX_TO_BLOCK[idx];
  get_block(block, puzzle, &house);
  empty_cell_count = 0;
  for (int k = 0; k < PUZZLE_WIDTH; ++k) {
    if (house.cells[k] == 0) {
      empty_cell_count++;
    }
    if (empty_cell_count > 1) {
      is_full_house_col = false;
      break;
    }
  }
  return is_full_house_block;
}
bool find_hidden_single(Puzzle* puzzle) {

}
