#include "singles.h"

#include <stdbool.h>
#include "puzzle.h"
#include "log.h"
#include "step.h"

TechiqueResult find_naked_single(Puzzle* puzzle) {
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(puzzle->cells[i] != 0) {
      continue;
    }
    if(__builtin_popcount(puzzle->candidates[i]) != 1) {
      continue;
    }
    uint8_t found_val = (uint8_t)__builtin_ctz(puzzle->candidates[i]) + 1;
    Step step = {
      .eliminated_mask = puzzle->candidates[i],
      .placed_val = found_val,
      .target_cell = (uint8_t)i,
      .technique = is_full_house(puzzle, i) ? FULL_HOUSE : NAKED_SINGLE
    };
    apply_step(puzzle, step);
    return PROGRESS_MADE;
  }
  return NO_PROGRESS;
}
bool is_full_house(Puzzle* puzzle, uint8_t idx) {
  bool is_full_house_row = true;
  bool is_full_house_col = true;
  bool is_full_house_block = true;

  uint8_t row = IDX_TO_ROW[idx];
  House house;
  get_row(row, puzzle, &house);
  uint8_t empty_cell_count = 0;
  for(int k = 0; k < PUZZLE_WIDTH; ++k) {
    if(house.cells[k] == 0) {
      empty_cell_count++;
    }
    if(empty_cell_count > 1) {
      is_full_house_row = false;
      break;
    }
  }
  if(is_full_house_row) {
    return true;
  }
  uint8_t col = IDX_TO_COL[idx];
  get_col(col, puzzle, &house);
  empty_cell_count = 0;
  for(int k = 0; k < PUZZLE_WIDTH; ++k) {
    if(house.cells[k] == 0) {
      empty_cell_count++;
    }
    if(empty_cell_count > 1) {
      is_full_house_col = false;
      break;
    }
  }
  if(is_full_house_col) {
    return true;
  }

  uint8_t block = IDX_TO_BLOCK[idx];
  get_block(block, puzzle, &house);
  empty_cell_count = 0;
  for(int k = 0; k < PUZZLE_WIDTH; ++k) {
    if(house.cells[k] == 0) {
      empty_cell_count++;
    }
    if(empty_cell_count > 1) {
      is_full_house_block = false;
      break;
    }
  }
  return is_full_house_block;
}
TechiqueResult find_hidden_single(Puzzle* puzzle) {
  House house;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(puzzle->cells[i] != 0) {
      continue;
    }
    uint16_t candidate_mask = puzzle->candidates[i];
    uint16_t other_cell_masks = 0;

    uint8_t row_idx = IDX_TO_ROW[i];
    uint8_t col_idx = IDX_TO_COL[i];
    get_row(row_idx, puzzle, &house);

    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      if(house.cells[k] != 0 || k == col_idx) {
        continue;
      }
      other_cell_masks |= house.candidates[k];
    }

    uint16_t unique = candidate_mask & ~other_cell_masks;
    if(unique) {
      if(__builtin_popcount(unique) != 1) {
        log_error("Error: Found multiple unique candiates in cell idx: %d", i);
        return INVALID_STATE;
      }
      int found_single = __builtin_ctz(unique) + 1;
      Step step = {
        .placed_val = found_single,
        .eliminated_mask = puzzle->candidates[i],
        .target_cell = (uint8_t)i,
        .technique = HIDDEN_SINGLE
      };
      apply_step(puzzle, step);
      return PROGRESS_MADE;
    }

    // Column search
    get_col(col_idx, puzzle, &house);
    other_cell_masks = 0;

    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      if(house.cells[k] != 0 || k == row_idx) {
        continue;
      }
      other_cell_masks |= house.candidates[k];
    }
    unique = candidate_mask & ~other_cell_masks;
    if(unique) {
      if(__builtin_popcount(unique) != 1) {
        log_error("Error: Found multiple unique candiates in cell idx: %d", i);
        return INVALID_STATE;
      }
      int found_single = __builtin_ctz(unique) + 1;
      Step step = {
        .placed_val = found_single,
        .eliminated_mask = puzzle->candidates[i],
        .target_cell = (uint8_t)i,
        .technique = HIDDEN_SINGLE
      };
      apply_step(puzzle, step);
      return PROGRESS_MADE;
    }

    uint8_t block_idx = IDX_TO_BLOCK[i];
    get_block(block_idx, puzzle, &house);
    other_cell_masks = 0;

    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      if(house.cells[k] != 0 || k == CELL_POS_IN_BLOCK[row_idx][col_idx]) {
        continue;
      }
      other_cell_masks |= house.candidates[k];
    }
    unique = candidate_mask & ~other_cell_masks;
    if(unique) {
      if(__builtin_popcount(unique) != 1) {
        log_error("Error: Found multiple unique candiates in cell idx: %d", i);
        return INVALID_STATE;
      }
      int found_single = __builtin_ctz(unique) + 1;
      Step step = {
        .placed_val = found_single,
        .eliminated_mask = puzzle->candidates[i],
        .target_cell = (uint8_t)i,
        .technique = HIDDEN_SINGLE
      };
      apply_step(puzzle, step);
      return PROGRESS_MADE;
    }
  }
  return NO_PROGRESS;
}

