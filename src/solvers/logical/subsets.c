#include <stdbool.h>
#include <stdint.h>
#include "subsets.h"
#include "utils.h"
#include "puzzle_globals.h"
TechiqueResult find_naked_pair(Puzzle* puzzle, House* house) {
  for(int i1 = 0; i1 < PUZZLE_WIDTH - 1; ++i1) {
    if(house->cells[i1] > 0 || __builtin_popcount(house->candidates[i1]) > 2) {
      continue;
    }
    for(int i2 = i1 + 1; i2 < PUZZLE_WIDTH; ++i2) {
      if(house->cells[i2] > 0 || __builtin_popcount(house->candidates[i2]) > 2) {
        continue;
      }
      uint16_t union_mask = house->candidates[i1] | house->candidates[i2];
      if(__builtin_popcount(union_mask) == 2) {
        bool progress_made = false;
        for(int i = 0; i < PUZZLE_WIDTH; ++i) {
          if(i == i1 || i == i2 || !(house->candidates[i] & union_mask)) {
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
  for(int i1 = 0; i1 < PUZZLE_WIDTH - 2; ++i1) {
    if(house->cells[i1] != 0 || __builtin_popcount(house->candidates[i1]) > 3) {
      continue;
    }
    for(int i2 = i1 + 1; i2 < PUZZLE_WIDTH - 1; ++i2) {
      if(house->cells[i2] != 0 || __builtin_popcount(house->candidates[i2]) > 3) {
        continue;
      }
      for(int i3 = i2 + 1; i3 < PUZZLE_WIDTH; ++i3) {
        if(house->cells[i3] != 0 || __builtin_popcount(house->candidates[i3]) > 3) {
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
    for(int i2 = i1 + 1; i2 < PUZZLE_WIDTH - 2; ++i2) {
      if(house->cells[i2] != 0 || __builtin_popcount(house->candidates[i2]) > 4) {
        continue;
      }
      for(int i3 = i2 + 1; i3 < PUZZLE_WIDTH - 1; ++i3) {
        if(house->cells[i3] != 0 || __builtin_popcount(house->candidates[i3]) > 4) {
          continue;
        }
        for(int i4 = i3 + 1; i4 < PUZZLE_WIDTH; ++i4) {
          if(house->cells[i4] != 0 || __builtin_popcount(house->candidates[i4]) > 4) {
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
  uint8_t empty_cells_idxs[9];
  int empty_cell_count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house->cells[i] != 0) {
      continue;
    }
    empty_cells_idxs[empty_cell_count++] = i;
    all_candidate_union |= house->candidates[i];
  }
  MaskArr all_candidate_arr = MASK_TO_ARRAY_LOOKUP[all_candidate_union];
  // it makes no sense to search unless there is one more cell empty than the pair
  if(empty_cell_count < 3) {
    return NO_PROGRESS;
  }
  for(int c1 = 0; c1 < all_candidate_arr.length - 1; ++c1) {
    for(int c2 = c1 + 1; c2 < all_candidate_arr.length; ++c2) {
      uint16_t potential_pair = (1 << (all_candidate_arr.candidates[c1] - 1)) | (1 << (all_candidate_arr.candidates[c2] - 1));
      uint8_t cells_with_pair_buffer[9];
      uint8_t cells_with_pair_count = 0;
      for(int i = 0; i < empty_cell_count; ++i) {
        uint16_t matching_candidates = potential_pair & house->candidates[empty_cells_idxs[i]];
        if(matching_candidates) {
          cells_with_pair_buffer[cells_with_pair_count] = empty_cells_idxs[i];
          cells_with_pair_count++;
          if(cells_with_pair_count > 2) {
            break;
          }
        }
      }
      if(cells_with_pair_count != 2) {
        continue;;
      }
      uint8_t idx1 = house->idx_lookup[cells_with_pair_buffer[0]];
      uint8_t idx2 = house->idx_lookup[cells_with_pair_buffer[1]];
      uint16_t removed1 = puzzle->candidates[idx1] & ~potential_pair;
      uint16_t removed2 = puzzle->candidates[idx2] & ~potential_pair;
      bool progress_made = false;

      if(removed1) {
        Step step = {
          .eliminated_mask = removed1,
          .placed_val = 0,
          .target_cell = idx1,
          .technique = HIDDEN_PAIR
        };
        apply_step(puzzle, step);
        progress_made = true;
      }
      if(removed2) {
        Step step = {
          .eliminated_mask = removed2,
          .placed_val = 0,
          .target_cell = idx2,
          .technique = HIDDEN_PAIR
        };
        apply_step(puzzle, step);
        progress_made = true;
      }
      if(progress_made) {
        return PROGRESS_MADE;
      }
    }
  }
  return NO_PROGRESS;
}

TechiqueResult find_hidden_triple(Puzzle* puzzle, House* house) {
  uint16_t all_candidate_union = 0;
  uint8_t empty_cells_idxs[9];
  int empty_cell_count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house->cells[i] != 0) {
      continue;
    }
    empty_cells_idxs[empty_cell_count++] = i;
    all_candidate_union |= house->candidates[i];
  }
  // it makes no sense to search unless there is one more empty cell than a triple
  if(empty_cell_count < 4) {
    return NO_PROGRESS;
  }
  MaskArr all_candidate_arr = MASK_TO_ARRAY_LOOKUP[all_candidate_union];
  for(int c1 = 0; c1 < all_candidate_arr.length - 2; ++c1) {
    for(int c2 = c1 + 1; c2 < all_candidate_arr.length - 1; ++c2) {
      for(int c3 = c2 + 1; c3 < all_candidate_arr.length; ++c3) {
        uint16_t potential_triple = (1 << (all_candidate_arr.candidates[c1] - 1))
          | (1 << (all_candidate_arr.candidates[c2] - 1))
          | (1 << (all_candidate_arr.candidates[c3] - 1));
        uint8_t cells_with_triple_buffer[9];
        uint8_t potential_triple_count = 0;
        for(int i = 0; i < empty_cell_count; ++i) {
          uint16_t matching_candidates = potential_triple & house->candidates[empty_cells_idxs[i]];
          if(matching_candidates) {
            cells_with_triple_buffer[potential_triple_count++] = empty_cells_idxs[i];
            if(potential_triple_count > 3) {
              break;
            }
          }
        }
        if(potential_triple_count != 3) {
          continue;
        }
        uint8_t idx1 = house->idx_lookup[cells_with_triple_buffer[0]];
        uint8_t idx2 = house->idx_lookup[cells_with_triple_buffer[1]];
        uint8_t idx3 = house->idx_lookup[cells_with_triple_buffer[2]];

        uint16_t removed1 = puzzle->candidates[idx1] & ~potential_triple;
        uint16_t removed2 = puzzle->candidates[idx2] & ~potential_triple;
        uint16_t removed3 = puzzle->candidates[idx3] & ~potential_triple;

        bool progress_made = false;
        if(removed1) {
          Step step = {
            .eliminated_mask = removed1,
            .placed_val = 0,
            .target_cell = idx1,
            .technique = HIDDEN_TRIPLE
          };
          apply_step(puzzle, step);
          progress_made = true;
        }
        if(removed2) {
          Step step = {
            .eliminated_mask = removed2,
            .placed_val = 0,
            .target_cell = idx2,
            .technique = HIDDEN_TRIPLE
          };
          apply_step(puzzle, step);
          progress_made = true;
        }
        if(removed3) {
          Step step = {
            .eliminated_mask = removed3,
            .placed_val = 0,
            .target_cell = idx3,
            .technique = HIDDEN_TRIPLE
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

TechiqueResult find_hidden_quad(Puzzle* puzzle, House* house) {
  uint16_t all_candidate_union = 0;
  uint8_t empty_cells_idxs[9];
  int empty_cell_count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house->cells[i] != 0) {
      continue;
    }
    empty_cells_idxs[empty_cell_count++] = i;
    all_candidate_union |= house->candidates[i];
  }
  // it makse no sense to search unless there is at least one more cell open than a quad
  if(empty_cell_count < 5) {
    return NO_PROGRESS;
  }
  MaskArr all_candidate_arr = MASK_TO_ARRAY_LOOKUP[all_candidate_union];

  for(int c1 = 0; c1 < all_candidate_arr.length - 3; ++c1) {
    for(int c2 = c1 + 1; c2 < all_candidate_arr.length - 2; ++c2) {
      for(int c3 = c2 + 1; c3 < all_candidate_arr.length - 1; ++c3) {
        for(int c4 = c3 + 1; c4 < all_candidate_arr.length; ++c4) {
          uint16_t potential_quad = (1 << (all_candidate_arr.candidates[c1] - 1))
            | (1 << (all_candidate_arr.candidates[c2] - 1))
            | (1 << (all_candidate_arr.candidates[c3] - 1))
            | (1 << (all_candidate_arr.candidates[c4] - 1));
          uint8_t cells_with_potential_quads[9];
          uint8_t potential_quads_count = 0;
          for(int i = 0; i < empty_cell_count; ++i) {
            uint16_t matching_candidate = house->candidates[empty_cells_idxs[i]] & potential_quad;
            if(matching_candidate) {
              cells_with_potential_quads[potential_quads_count++] = empty_cells_idxs[i];
              if(potential_quads_count > 4) {
                break;
              }
            }
          }
          if(potential_quads_count != 4) {
            continue;
          }
          uint8_t idx1 = house->idx_lookup[cells_with_potential_quads[0]];
          uint8_t idx2 = house->idx_lookup[cells_with_potential_quads[1]];
          uint8_t idx3 = house->idx_lookup[cells_with_potential_quads[2]];
          uint8_t idx4 = house->idx_lookup[cells_with_potential_quads[3]];

          uint16_t removed1 = puzzle->candidates[idx1] & ~potential_quad;
          uint16_t removed2 = puzzle->candidates[idx2] & ~potential_quad;
          uint16_t removed3 = puzzle->candidates[idx3] & ~potential_quad;
          uint16_t removed4 = puzzle->candidates[idx4] & ~potential_quad;
          bool progress_made = false;
          if(removed1) {
            Step step = {
              .eliminated_mask = removed1,
              .placed_val = 0,
              .target_cell = idx1,
              .technique = HIDDEN_QUAD
            };
            apply_step(puzzle, step);
            progress_made = true;
          }
          if(removed2) {
            Step step = {
              .eliminated_mask = removed2,
              .placed_val = 0,
              .target_cell = idx2,
              .technique = HIDDEN_QUAD
            };
            apply_step(puzzle, step);
            progress_made = true;
          }
          if(removed3) {
            Step step = {
              .eliminated_mask = removed3,
              .placed_val = 0,
              .target_cell = idx3,
              .technique = HIDDEN_QUAD
            };
            apply_step(puzzle, step);
            progress_made = true;
          }
          if(removed4) {
            Step step = {
              .eliminated_mask = removed4,
              .placed_val = 0,
              .target_cell = idx4,
              .technique = HIDDEN_QUAD
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
