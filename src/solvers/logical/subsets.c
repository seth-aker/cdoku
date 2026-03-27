#include <stdbool.h>
#include <stdint.h>
#include "subsets.h"
#include "utils.h"
#include "globals.h"
TechiqueResult find_naked_pair(Puzzle* puzzle, House* house) {
  for (int i1 = 0; i1 < PUZZLE_WIDTH - 1; ++i1) {
    if (house->cells[i1] > 0 || __builtin_popcount(house->candidates[i1]) > 2) {
      continue;
    }
    for (int i2 = i1 + 1; i2 < PUZZLE_WIDTH; ++i2) {
      if (house->cells[i2] > 0 || __builtin_popcount(house->candidates[i2]) > 2) {
        continue;
      }
      uint16_t union_mask = house->candidates[i1] | house->candidates[i2];
      if (__builtin_popcount(union_mask) == 2) {
        bool progress_made = false;
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
        if(progress_made) {
          return PROGRESS_MADE;
        }
      }
    }
  }
  return NO_PROGRESS;
}

TechiqueResult find_naked_triple(Puzzle* puzzle, House* house) {
  for (int i1 = 0; i1 < PUZZLE_WIDTH - 2; ++i1) {
    if (house->cells[i1] != 0 || __builtin_popcount(house->candidates[i1]) > 3) {
      continue;
    }
    for (int i2 = i1 + 1; i2 < PUZZLE_WIDTH - 1; ++i2) {
      if (house->cells[i2] != 0 || __builtin_popcount(house->candidates[i2]) > 3) {
        continue;
      }
      for (int i3 = i2 + 1; i3 < PUZZLE_WIDTH; ++i3) {
        if (house->cells[i3] != 0 || __builtin_popcount(house->candidates[i3]) > 3) {
          continue;
        }
        uint16_t union_mask = house->candidates[i1] | house->candidates[i2] | house->candidates[i3];
        if(__builtin_popcount(union_mask) == 3) {
          bool progress_made = false;
          for(int i = 0; i < PUZZLE_WIDTH; ++i) {
            if(i1 == i || i2 == i || i3 == i || !(house->candidates[i] & union_mask)) {
              continue;
            }
            Step step = {
              .eliminated_mask = house->candidates[i] & union_mask,
              .placed_val = 0,
              .target_cell = house->idx_lookup[i],
              .technique = NAKED_TRIPLE
            };
            apply_step(puzzle, step);
            progress_made = true;
          }
          if(progress_made) {
            return PROGRESS_MADE;
          }
        }
      }
    }
  }
  return NO_PROGRESS;
}

TechiqueResult find_naked_quad(Puzzle* puzzle, House* house) {
  for(int i1 = 0; i1 < PUZZLE_WIDTH - 3; ++i1) {
    if(house->cells[i1] != 0 || __builtin_popcount(house->candidates[i1]) > 4) {
      continue;
    }
    for (int i2 = i1 + 1; i2 < PUZZLE_WIDTH - 2; ++i2) {
      if (house->cells[i2] != 0 || __builtin_popcount(house->candidates[i2]) > 4) {
        continue;
      }
      for (int i3 = i2 + 1; i3 < PUZZLE_WIDTH - 1; ++i3) {
        if (house->cells[i3] != 0 || __builtin_popcount(house->candidates[i3]) > 4) {
          continue;
        }
        for(int i4 = i3 + 1; i4 < PUZZLE_WIDTH; ++i4) {
          if (house->cells[i4] != 0 || __builtin_popcount(house->candidates[i4]) > 4) {
            continue;
          }
          uint16_t union_mask = house->candidates[i1] | house->candidates[i2] | house->candidates[i3] | house->candidates[i4];
          if(__builtin_popcount(union_mask) == 4) {
            bool progress_made = false;
            for(int i = 0; i < PUZZLE_WIDTH; ++i) {
              if(i == i1 || i == i2 || i == i3 || i == i4 || !(house->candidates[i] & union_mask)) {
                continue;
              }
              Step step = {
                .eliminated_mask = house->candidates[i] & union_mask,
                .placed_val = 0,
                .target_cell = house->idx_lookup[i],
                .technique = NAKED_QUAD
              };
              apply_step(puzzle, step);
              progress_made = true;
            }
            if(progress_made) {
              return PROGRESS_MADE;
            }
          }
        }
      }
    }
  }
  return NO_PROGRESS;
}

TechiqueResult find_hidden_pair(Puzzle* puzzle, House* house) {
  uint16_t all_candidate_union = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house->cells[i] != 0) {
      continue;
    }
    all_candidate_union |= house->candidates[i];
  }
  MaskArr all_candidate_arr = MASK_TO_ARRAY_LOOKUP[all_candidate_union];

  for(int c1 = 0; c1 < all_candidate_arr.length - 1; ++c1) {
    for(int c2 = c1 + 1; c2 < all_candidate_arr.length; ++c2) {
      uint16_t potential_pair = (1 << all_candidate_arr.candidates[c1]) | (1 << all_candidate_arr.candidates[c2]);
      uint8_t cells_with_pair_buffer[9];
      uint8_t cells_with_pair_count = 0;
      for(int i = 0; i < PUZZLE_WIDTH; ++i) {
        if(house->cells[i] == 0) {
          continue;
        }
        if(potential_pair & house->candidates[i]) {
          cells_with_pair_buffer[cells_with_pair_count] = i;
          cells_with_pair_count++;
          if(cells_with_pair_count > 2) {
            break;
          } 
        }
      }
      if(cells_with_pair_count != 2) {
        continue;
      }

    }
  }
}