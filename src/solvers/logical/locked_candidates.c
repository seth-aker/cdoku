#include "locked_candidates.h"
#include "puzzle_globals.h"
#include <stdbool.h>
#include <stdint.h>

TechiqueResult find_locked_candidate_pointing(Puzzle* puzzle) {
  House block;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    get_block(i, puzzle, &block);
    uint16_t row1 = block.candidates[0] | block.candidates[1] | block.candidates[2];
    uint16_t row2 = block.candidates[3] | block.candidates[4] | block.candidates[5];
    uint16_t row3 = block.candidates[6] | block.candidates[7] | block.candidates[8];

    uint16_t unique_row1 = row1 & ~(row2 | row3);
    uint16_t unique_row2 = row2 & ~(row1 | row3);
    uint16_t unique_row3 = row3 & ~(row1 | row2);

    if(unique_row1 && remove_pointing_row(puzzle, unique_row1, i, 0)) {
      return PROGRESS_MADE;
    }
    if(unique_row2 && remove_pointing_row(puzzle, unique_row2, i, 1)) {
      return PROGRESS_MADE;
    }
    if(unique_row3 && remove_pointing_row(puzzle, unique_row3, i, 2)) {
      return PROGRESS_MADE;
    }

    uint16_t col1 = block.candidates[0] | block.candidates[3] | block.candidates[6];
    uint16_t col2 = block.candidates[1] | block.candidates[4] | block.candidates[7];
    uint16_t col3 = block.candidates[2] | block.candidates[5] | block.candidates[8];

    uint16_t unique_col1 = col1 & ~(col2 | col3);
    uint16_t unique_col2 = col2 & ~(col1 | col3);
    uint16_t unique_col3 = col3 & ~(col1 | col2);

    if(unique_col1 && remove_pointing_col(puzzle, unique_col1, i, 0)) {
      return PROGRESS_MADE;
    }
    if(unique_col2 && remove_pointing_col(puzzle, unique_col2, i, 1)) {
      return PROGRESS_MADE;
    }
    if(unique_col3 && remove_pointing_col(puzzle, unique_col3, i, 2)) {
      return PROGRESS_MADE;
    }
  }
  return NO_PROGRESS;
}

bool remove_pointing_row(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t block_row_idx) {
  uint8_t idx_offset = (BLOCK_IDX_TO_START_ROW[block_idx] + block_row_idx) * 9;

  bool progress_made = false;
  uint8_t block_start_col = BLOCK_IDX_TO_START_COL[block_idx];
  for(int col = 0; col < block_start_col; ++col) {
    uint8_t puzzle_idx = idx_offset + col;
    uint16_t removed = puzzle->candidates[puzzle_idx] & mask_to_remove;
    if(removed) {
      Step step = {
        .eliminated_mask = removed,
        .placed_val = 0,
        .target_cell = puzzle_idx,
        .technique = LOCKED_POINTING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  for(int col = block_start_col + 3; col < PUZZLE_WIDTH; ++col) {
    uint8_t puzzle_idx = idx_offset + col;
    uint16_t removed = puzzle->candidates[puzzle_idx] & mask_to_remove;
    if(removed) {
      Step step = {
        .eliminated_mask = removed,
        .placed_val = 0,
        .target_cell = puzzle_idx,
        .technique = LOCKED_POINTING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  return progress_made;
}
bool remove_pointing_col(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t block_col_idx) {
  uint8_t block_start_row = BLOCK_IDX_TO_START_ROW[block_idx];
  uint8_t block_col_offset = BLOCK_IDX_TO_START_COL[block_idx] + block_col_idx;
  bool progress_made = false;
  for(int row = 0; row < block_start_row; ++row) {
    uint8_t puzzle_idx = (row * 9) + block_col_offset;
    uint16_t removed = puzzle->candidates[puzzle_idx] & mask_to_remove;
    if(removed) {
      Step step = {
        .eliminated_mask = removed,
        .placed_val = 0,
        .target_cell = puzzle_idx,
        .technique = LOCKED_POINTING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  for(int row = block_start_row + 3; row < PUZZLE_WIDTH; ++row) {
    uint8_t puzzle_idx = (row * 9) + block_col_offset;
    uint16_t removed = puzzle->candidates[puzzle_idx] & mask_to_remove;
    if(removed) {
      Step step = {
        .eliminated_mask = removed,
        .placed_val = 0,
        .target_cell = puzzle_idx,
        .technique = LOCKED_POINTING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  return progress_made;
}

TechiqueResult find_locked_candidate_claiming(Puzzle* puzzle) {
  const uint8_t* house;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    house = ROW_TO_IDXS[i];
    uint16_t row_seg1 = puzzle->candidates[house[0]] | puzzle->candidates[house[1]] | puzzle->candidates[house[2]];
    uint16_t row_seg2 = puzzle->candidates[house[3]] | puzzle->candidates[house[4]] | puzzle->candidates[house[5]];
    uint16_t row_seg3 = puzzle->candidates[house[6]] | puzzle->candidates[house[7]] | puzzle->candidates[house[8]];

    uint16_t unique_seg1 = row_seg1 & ~(row_seg2 | row_seg3);
    uint16_t unique_seg2 = row_seg2 & ~(row_seg1 | row_seg3);
    uint16_t unique_seg3 = row_seg3 & ~(row_seg1 | row_seg2);

    uint8_t block_idx_offset = (i / 3) * 3;
    uint8_t local_block_pos = i % 3;
    uint8_t skip_row_start = local_block_pos * 3;
    if(unique_seg1 && remove_claiming_row(puzzle, unique_seg1, (block_idx_offset), skip_row_start)) {
      return PROGRESS_MADE;
    }
    if(unique_seg2 && remove_claiming_row(puzzle, unique_seg2, (block_idx_offset + 1), skip_row_start)) {
      return PROGRESS_MADE;
    }
    if(unique_seg3 && remove_claiming_row(puzzle, unique_seg3, (block_idx_offset + 2), skip_row_start)) {
      return PROGRESS_MADE;
    }
    // -------------
    // COLUMNS
    // -------------
    house = COL_TO_IDXS[i];
    uint16_t col_seg1 = puzzle->candidates[house[0]] | puzzle->candidates[house[1]] | puzzle->candidates[house[2]];
    uint16_t col_seg2 = puzzle->candidates[house[3]] | puzzle->candidates[house[4]] | puzzle->candidates[house[5]];
    uint16_t col_seg3 = puzzle->candidates[house[6]] | puzzle->candidates[house[7]] | puzzle->candidates[house[8]];

    unique_seg1 = col_seg1 & ~(col_seg2 | col_seg3);
    unique_seg2 = col_seg2 & ~(col_seg1 | col_seg3);
    unique_seg3 = col_seg3 & ~(col_seg1 | col_seg2);

    block_idx_offset = (i / 3);
    if(unique_seg1 && remove_claiming_col(puzzle, unique_seg1, block_idx_offset, local_block_pos)) {
      return PROGRESS_MADE;
    }
    if(unique_seg2 && remove_claiming_col(puzzle, unique_seg2, block_idx_offset + 3, local_block_pos)) {
      return PROGRESS_MADE;
    }
    if(unique_seg3 && remove_claiming_col(puzzle, unique_seg3, block_idx_offset + 6, local_block_pos)) {
      return PROGRESS_MADE;
    }
  }
  return NO_PROGRESS;
}

bool remove_claiming_row(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t skip_row_start) {
  const uint8_t* block = BLOCK_TO_IDXS[block_idx];
  bool progress_made = false;
  for(int i = 0; i < skip_row_start; ++i) {
    uint8_t idx = block[i];
    uint16_t removed = puzzle->candidates[idx] & mask_to_remove;
    if(removed) {
      Step step = {
        .eliminated_mask = removed,
        .placed_val = 0,
        .target_cell = idx,
        .technique = LOCKED_CLAIMING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  for(int i = skip_row_start + 3; i < PUZZLE_WIDTH; ++i) {
    uint8_t idx = block[i];
    uint16_t removed = puzzle->candidates[idx] & mask_to_remove;
    if(removed) {
      Step step = {
        .eliminated_mask = removed,
        .placed_val = 0,
        .target_cell = idx,
        .technique = LOCKED_CLAIMING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  return progress_made;
}

bool remove_claiming_col(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t skip_col_idx) {
  bool progress_made = false;
  const uint8_t* block = BLOCK_TO_IDXS[block_idx];
  // If skip is 0, colA is 1, colB is 2
  // If skip is 1, colA is 2, colB is 0
  // If skip is 2, colA is 0, colB is 1
  uint8_t colA = (skip_col_idx + 1) % 3;
  uint8_t colB = (skip_col_idx + 2) % 3;

  for(int r = 0; r < 3; ++r) {
    uint8_t idx_offset = r * 3;
    uint16_t removed1 = puzzle->candidates[block[idx_offset + colA]] & mask_to_remove;
    uint16_t removed2 = puzzle->candidates[block[idx_offset + colB]] & mask_to_remove;
    if(removed1) {
      Step step = {
        .eliminated_mask = removed1,
        .placed_val = 0,
        .target_cell = block[idx_offset + colA],
        .technique = LOCKED_CLAIMING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
    if(removed2) {
      Step step = {
        .eliminated_mask = removed2,
        .placed_val = 0,
        .target_cell = block[idx_offset + colB],
        .technique = LOCKED_CLAIMING
      };
      apply_step(puzzle, step);
      progress_made = true;
    }
  }
  return progress_made;
}
