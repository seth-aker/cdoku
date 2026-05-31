#include <stdbool.h>
#include "x_wing.h"
#include "step.h"
#include "puzzle.h"
#include "utils.h"

TechniqueResult find_x_wing(Puzzle* puzzle) {
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    uint16_t row_masks[9] = { 0 };
    uint16_t col_masks[9] = { 0 };
    uint16_t candidate_mask = 1 << (candidate - 1);
    for(int i = 0; i < 81; ++i) {
      if(puzzle->candidates[i] & candidate_mask) {
        row_masks[IDX_TO_ROW[i]] |= (1 << IDX_TO_COL[i]);
        col_masks[IDX_TO_COL[i]] |= (1 << IDX_TO_ROW[i]);
      }
    }
    for(int r1 = 0; r1 < 9 - 1; ++r1) {
      uint16_t cover_cols_1 = row_masks[r1];
      int cover_count = __builtin_popcount(cover_cols_1);
      if(cover_count != 2) continue;
      for(int r2 = r1 + 1; r2 < 9; ++r2) {
        uint16_t cover_cols_2 = row_masks[r2];

        cover_count = __builtin_popcount(cover_cols_2);
        if(cover_count != 2) continue;
        uint16_t cover_union = cover_cols_1 | cover_cols_2;
        if(__builtin_popcount(cover_union) == 2) {
          uint16_t base_houses = (1 << r1) | (1 << r2);
          if(eliminate_fish(puzzle, candidate_mask, cover_union, base_houses, false)) {
            return PROGRESS_MADE;
          }
        }
      }
    }

    for(int c1 = 0; c1 < 9 - 1; ++c1) {
      uint16_t cover_rows_1 = col_masks[c1];
      int cover_count = __builtin_popcount(cover_rows_1);
      if(cover_count != 2) continue;

      for(int c2 = c1 + 1; c2 < 9; ++c2) {
        uint16_t cover_rows_2 = col_masks[c2];
        cover_count = __builtin_popcount(cover_rows_2);
        if(cover_count != 2) continue;

        uint16_t cover_union = cover_rows_1 | cover_rows_2;
        if(__builtin_popcount(cover_union) == 2) {
          uint16_t base_houses = (1 << c1) | (1 << c2);
          if(eliminate_fish(puzzle, candidate_mask, cover_union, base_houses, true)) {
            return PROGRESS_MADE;
          }
        }
      }
    }
  }
  return NO_PROGRESS;
}
