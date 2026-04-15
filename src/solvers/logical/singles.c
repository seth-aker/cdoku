#include "singles.h"

#include <stdbool.h>
#include "puzzle.h"
#include "log.h"
#include "step.h"

TechniqueResult find_naked_single(Puzzle* puzzle) {
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(puzzle->cells[i] != 0) {
      continue;
    }
    if(__builtin_popcount(puzzle->candidates[i]) != 1) {
      continue;
    }
    uint8_t found_val = (uint8_t)(__builtin_ctz(puzzle->candidates[i]) + 1);
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

  uint8_t row_idx = IDX_TO_ROW[idx];

  const uint8_t* row = ROW_TO_IDXS[row_idx];
  uint8_t empty_cell_count = 0;
  for(int k = 0; k < PUZZLE_WIDTH; ++k) {
    if(puzzle->cells[row[k]] == 0) {
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
  uint8_t col_idx = IDX_TO_COL[idx];
  const uint8_t* col = COL_TO_IDXS[col_idx];
  empty_cell_count = 0;
  for(int k = 0; k < PUZZLE_WIDTH; ++k) {
    if(puzzle->cells[col[k]] == 0) {
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

  uint8_t block_idx = IDX_TO_BLOCK[idx];
  const uint8_t* block = BLOCK_TO_IDXS[block_idx];
  empty_cell_count = 0;
  for(int k = 0; k < PUZZLE_WIDTH; ++k) {
    if(puzzle->cells[block[k]] == 0) {
      empty_cell_count++;
    }
    if(empty_cell_count > 1) {
      is_full_house_block = false;
      break;
    }
  }
  return is_full_house_block;
}
TechniqueResult find_hidden_single(Puzzle* puzzle) {
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(puzzle->cells[i] != 0) {
      continue;
    }
    uint16_t candidate_mask = puzzle->candidates[i];
    uint16_t other_cell_masks = 0;

    uint8_t row_idx = IDX_TO_ROW[i];

    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      uint8_t cell_in_row = ROW_TO_IDXS[row_idx][k];
      if(puzzle->cells[cell_in_row] != 0 || cell_in_row == i) {
        continue;
      }
      other_cell_masks |= puzzle->candidates[cell_in_row];
    }

    uint16_t unique = candidate_mask & ~other_cell_masks;
    if(unique) {
      if(__builtin_popcount(unique) != 1) {
        log_debug("Error: Found multiple unique candiates in cell idx: %d", i);
#ifndef NDEBUG
        print_puzzle_state(puzzle);
#endif
        return INVALID_STATE;
      }
      uint8_t found_single = (uint8_t)(__builtin_ctz(unique) + 1);
      Step step = {
        .placed_val = found_single,
        .eliminated_mask = unique,
        .target_cell = (uint8_t)i,
        .technique = HIDDEN_SINGLE
      };
      apply_step(puzzle, step);
      return PROGRESS_MADE;
    }

    // Column search
    uint8_t col_idx = IDX_TO_COL[i];
    other_cell_masks = 0;

    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      uint8_t cell_in_col = COL_TO_IDXS[col_idx][k];
      if(puzzle->cells[cell_in_col] != 0 || cell_in_col == i) {
        continue;
      }
      other_cell_masks |= puzzle->candidates[cell_in_col];
    }
    unique = candidate_mask & ~other_cell_masks;
    if(unique) {
      if(__builtin_popcount(unique) != 1) {
        log_error("Error: Found multiple unique candiates in cell idx: %d", i);
        print_puzzle_state(puzzle);
        return INVALID_STATE;
      }
      int found_single = __builtin_ctz(unique) + 1;
      Step step = {
        .placed_val = (uint8_t)found_single,
        .eliminated_mask = unique,
        .target_cell = (uint8_t)i,
        .technique = HIDDEN_SINGLE
      };
      apply_step(puzzle, step);
      return PROGRESS_MADE;
    }

    uint8_t block_idx = IDX_TO_BLOCK[i];

    other_cell_masks = 0;

    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      uint8_t peer_idx = BLOCK_TO_IDXS[block_idx][k];
      if(puzzle->cells[peer_idx] != 0 || peer_idx == i) {
        continue;
      }
      other_cell_masks |= puzzle->candidates[peer_idx];
    }
    unique = candidate_mask & ~other_cell_masks;
    if(unique) {
      if(__builtin_popcount(unique) != 1) {
        log_error("Error: Found multiple unique candiates in cell idx: %d", i);
        print_puzzle_state(puzzle);
        return INVALID_STATE;
      }
      int found_single = __builtin_ctz(unique) + 1;
      Step step = {
        .placed_val = (uint8_t)found_single,
        .eliminated_mask = unique,
        .target_cell = (uint8_t)i,
        .technique = HIDDEN_SINGLE
      };
      apply_step(puzzle, step);
      return PROGRESS_MADE;
    }
  }
  return NO_PROGRESS;
}

