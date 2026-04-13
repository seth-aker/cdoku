#include <stdbool.h>
#include "x_wing.h"
#include "step.h"
#include "puzzle.h"
#include "utils.h"

TechniqueResult find_x_wing(Puzzle* puzzle) {
  const int MAX_CAND_IN_HOUSE = 2;
  XWing_Context search_context;
  bool progress_made = false;
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    search_context.base_house_count = 0;
    uint16_t candidate_mask = 1 << (candidate - 1);
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      const uint8_t* house_candidates_idxs = ROW_TO_IDXS[i];
      int positions[2];
      SearchParams params = {
        .mask = candidate_mask,
        .max_count = MAX_CAND_IN_HOUSE
      };
      int cand_count = get_candidate_positions(puzzle, house_candidates_idxs, params, positions);
      if(cand_count == MAX_CAND_IN_HOUSE) {
        int base_idx = search_context.base_house_count;
        search_context.base_house_idx[base_idx] = i;
        search_context.cover_houses[base_idx][0] = positions[0];
        search_context.cover_houses[base_idx][1] = positions[1];
        search_context.base_house_count++;
      }
    }

    for(int r1 = 0; r1 < search_context.base_house_count - 1; ++r1) {
      for(int r2 = r1 + 1; r2 < search_context.base_house_count; ++r2) {
        const int* cover_col_1 = search_context.cover_houses[r1];
        const int* cover_col_2 = search_context.cover_houses[r2];
        // if the two don't match
        if(cover_col_1[0] != cover_col_2[0] || cover_col_1[1] != cover_col_2[1]) {
          continue;
        }

        for(int row_idx = 0; row_idx < PUZZLE_WIDTH; ++row_idx) {
          // if row_idx is one of the base rows skip it
          if(row_idx == search_context.base_house_idx[r1] || row_idx == search_context.base_house_idx[r2]) {
            continue;
          }
          uint8_t idx_1 = ROW_TO_IDXS[row_idx][cover_col_1[0]];
          uint8_t idx_2 = ROW_TO_IDXS[row_idx][cover_col_1[1]];
          // cover col 1
          uint16_t removed = puzzle->candidates[idx_1] & candidate_mask;
          if(removed) {
            Step newStep = {
              .eliminated_mask = removed,
              .placed_val = 0,
              .target_cell = idx_1,
              .technique = X_WING
            };
            apply_step(puzzle, newStep);
            progress_made = true;
          }
          // cover col 2
          removed = puzzle->candidates[idx_2] & candidate_mask;
          if(removed) {
            Step newStep = {
              .eliminated_mask = removed,
              .placed_val = 0,
              .target_cell = idx_2,
              .technique = X_WING
            };
            apply_step(puzzle, newStep);
            progress_made = true;
          }
        }
        if(progress_made) {
          return PROGRESS_MADE;
        }
      }
    }

    // reset and then look for columns
    search_context.base_house_count = 0;
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      const uint8_t* house_candidates_idxs = COL_TO_IDXS[i];
      int positions[2];
      SearchParams params = {
        .mask = candidate_mask,
        .max_count = MAX_CAND_IN_HOUSE
      };

      int cand_count = get_candidate_positions(puzzle, house_candidates_idxs, params, positions);
      if(cand_count == MAX_CAND_IN_HOUSE) {
        int base_idx = search_context.base_house_count;
        search_context.base_house_idx[base_idx] = i;
        search_context.cover_houses[base_idx][0] = positions[0];
        search_context.cover_houses[base_idx][1] = positions[1];
        search_context.base_house_count++;
      }
    }

    for(int c1 = 0; c1 < search_context.base_house_count - 1; ++c1) {
      for(int c2 = c1 + 1; c2 < search_context.base_house_count; ++c2) {
        const int* cover_row_1 = search_context.cover_houses[c1];
        const int* cover_row_2 = search_context.cover_houses[c2];
        if(cover_row_1[0] != cover_row_2[0] || cover_row_1[1] != cover_row_2[1]) {
          continue;
        }
        for(int col_idx = 0; col_idx < PUZZLE_WIDTH; ++col_idx) {
          // if col index is one of the base cols
          if(col_idx == search_context.base_house_idx[c1] || col_idx == search_context.base_house_idx[c2]) {
            continue;
          }
          uint8_t idx_1 = COL_TO_IDXS[col_idx][cover_row_1[0]];
          uint8_t idx_2 = COL_TO_IDXS[col_idx][cover_row_1[1]];

          uint16_t removed = puzzle->candidates[idx_1] & candidate_mask;
          if(removed) {
            Step newStep = {
              .eliminated_mask = removed,
              .placed_val = 0,
              .target_cell = idx_1,
              .technique = X_WING
            };
            apply_step(puzzle, newStep);
            progress_made = true;
          }
          removed = puzzle->candidates[idx_2] & candidate_mask;
          if(removed) {
            Step newStep = {
              .eliminated_mask = removed,
              .placed_val = 0,
              .target_cell = idx_2,
              .technique = X_WING
            };
            apply_step(puzzle, newStep);
            progress_made = true;
          }
        }
        if(progress_made) {
          return PROGRESS_MADE;
        }
      }
    }
  }
  return NO_PROGRESS;
}

