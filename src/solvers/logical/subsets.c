#include "subsets.h"
#include "utils.h"
#include <stdbool.h>
TechiqueResult find_naked_pair(Puzzle* puzzle, House* house) {
  bool progress_made = false;
  for (int i1 = 0; i1 < PUZZLE_WIDTH - 1; ++i1) {
    if (house->cells[i1] > 0 || __builtin_popcount(house->candidates[i1]) > 2) {
      continue;
    }
    for (int i2 = 1; i2 < PUZZLE_WIDTH; ++i2) {
      if (house->cells[i2] > 0 || __builtin_popcount(house->candidates[i2]) > 2) {
        continue;
      }
      uint16_t union_mask = house->candidates[i1] | house->candidates[i2];
      if (__builtin_popcount(union_mask) == 2) {
        for (int i = 0; i < PUZZLE_WIDTH; ++i) {
          if (i == i1 || i == i2 || !(house->candidates[i] & union_mask)) {
            continue;
          }

          Step step = {
            .eliminated_mask = house->candidates[i] & union_mask,
            .placed_val = 0,
            .target_cell = house->idx_lookup[i],
            .technique = NAKED_PAIR
          };
          apply_step(puzzle, step);
          progress_made = true;
        }
      }
    }
  }
  return progress_made ? PROGRESS_MADE : NO_PROGRESS;
}

TechiqueResult find_naked_triple(Puzzle* puzzle, House* house) {
  bool progress_made = false;
  for (int i1 = 0; i1 < PUZZLE_WIDTH - 2; ++i1) {
    if (house->cells[i1] != 0 || __builtin_popcount(house->candidates[i1] > 3)) {
      continue;
    }
    for (int i2 = 1; i2 < PUZZLE_WIDTH - 1; ++i2) {
      if (house->cells[i2] != 0 || __builtin_popcount(house->candidates[i2] > 3)) {
        continue;
      }
      for (int i3 = 2; i3 < PUZZLE_WIDTH; ++i3) {
        if (house->cells[i3] != 0 || __builtin_popcount(house->candidates[i3] > 3)) {
          continue;
        }

      }
    }
  }
}
