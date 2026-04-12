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
    search_context.candidate_mask = 1 << (candidate - 1);
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      const uint8_t* house_candidates_idxs = ROW_TO_IDXS[i];
      int cand_count = count_candidate_in_house(puzzle, house_candidates_idxs, &search_context, MAX_CAND_IN_HOUSE);
      if(cand_count == MAX_CAND_IN_HOUSE) {
        search_context.base_house_idx[search_context.base_house_count++] = i;
      }
    }

    for(int r1 = 0; r1 < search_context.base_house_count - 1; ++r1) {
      const int* cover_col_1 = search_context.cover_cols[r1];
      for(int r2 = r1 + 1; r2 < search_context.base_house_count; ++r2) {
        const int* cover_col_2 = search_context.cover_cols[r2];
        // if the two don't match
        if(cover_col_1[0] != cover_col_2[0] || cover_col_1[1] != cover_col_2[1]) {
          continue;
        }

        for(int row_idx = 0; row_idx < PUZZLE_WIDTH; ++row_idx) {
          if(row_idx == search_context.base_house_idx[r1] || row_idx == search_context.base_house_idx[r2]) {
            continue;
          }
          uint8_t idx_1 = ROW_TO_IDXS[row_idx][cover_col_1[0]];
          uint8_t idx_2 = ROW_TO_IDXS[row_idx][cover_col_2[1]];
          // cover col 1
          uint16_t removed = puzzle->candidates[idx_1] & search_context.candidate_mask;
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
          removed = puzzle->candidates[idx_2] & search_context.candidate_mask;
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
      }
    }
    if(progress_made) {
      return PROGRESS_MADE;
    }
    // reset and then look for columns
    search_context.base_house_count = 0;
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      const uint8_t* house_candidates_idxs = COL_TO_IDXS[i];
      int cand_count = count_candidate_in_house(puzzle, house_candidates_idxs, &search_context, MAX_CAND_IN_HOUSE);
      if(cand_count == MAX_CAND_IN_HOUSE) {
        search_context.base_house_idx[search_context.base_house_count++] = i;
      }
    }
  }
  return NO_PROGRESS;
}

int count_candidate_in_house(Puzzle* puzzle, const uint8_t house_cand_idxs[], XWing_Context* search_context, int max_count) {
  int candidate_count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    uint16_t has_candidate = puzzle->candidates[house_cand_idxs[i]] & search_context->candidate_mask;
    if(has_candidate) {
      candidate_count++;
      if(candidate_count > max_count) {
        return candidate_count;
      }
      search_context->cover_cols[search_context->base_house_count][candidate_count - 1] = i;

    }
  }
  return candidate_count;
}
