#include "chute_remote_pairs.h"
#include "puzzle.h"
#include "utils.h"
// This runs with the assumption that all potential naked pairs have been found. 
TechniqueResult find_chute_remote_pairs(Puzzle* puzzle) {
  uint8_t bivalue_bins[512][18];
  uint8_t bin_count[512] = { 0 };
  for(int i = 0; i < 81; ++i) {
    if(puzzle->cells[i] != 0 || __builtin_popcount(puzzle->candidates[i]) != 2) {
      continue;
    }
    uint16_t mask = puzzle->candidates[i];
    bivalue_bins[mask][bin_count[mask]++] = i;
  }

  for(int bi = 0; bi < 512; ++bi) {
    if(bin_count[bi] < 2) continue;

    for(int i = 0; i < bin_count[bi] - 1; ++i) {
      for(int j = i + 1; j < bin_count[bi]; j++) {
        uint8_t idx_one = bivalue_bins[bi][i];
        uint8_t idx_two = bivalue_bins[bi][j];
        uint8_t h1 = IDX_TO_CHUTE_HOR[idx_one];
        uint8_t h2 = IDX_TO_CHUTE_HOR[idx_two];
        uint8_t v1 = IDX_TO_CHUTE_VERT[idx_one];
        uint8_t v2 = IDX_TO_CHUTE_VERT[idx_two];

        uint8_t r1 = IDX_TO_ROW[idx_one];
        uint8_t r2 = IDX_TO_ROW[idx_two];
        if(h1 == h2 && v1 != v2 && r1 != r2) {
          // check for chute remote pair
          uint8_t unused_row = (h1 * 9 + 3) - r1 - r2; // weird magic math I learned from the internet

          uint8_t v_unused = 3 - IDX_TO_CHUTE_VERT[idx_one] - IDX_TO_CHUTE_VERT[idx_two];

          uint8_t cell_one = (unused_row * 9) + (3 * v_unused);
          uint8_t cell_two = cell_one + 1;
          uint8_t cell_three = cell_one + 2;

          uint16_t mask_one = puzzle->cells[cell_one] != 0 ? (1 << (puzzle->cells[cell_one] - 1)) : puzzle->candidates[cell_one];
          uint16_t mask_two = puzzle->cells[cell_two] != 0 ? (1 << (puzzle->cells[cell_two] - 1)) : puzzle->candidates[cell_two];
          uint16_t mask_three = puzzle->cells[cell_three] != 0 ? (1 << (puzzle->cells[cell_three] - 1)) : puzzle->candidates[cell_three];
          uint16_t overlap_mask = puzzle->candidates[idx_one] & (mask_one | mask_two | mask_three);
          int overlap_count = __builtin_popcount(overlap_mask);
          if(overlap_count == 1) {
            uint16_t eliminate_mask = overlap_mask;
            if(eliminate_from_intersections(puzzle, eliminate_mask, idx_one, idx_two)) {
              return PROGRESS_MADE;
            }
          } else if(overlap_count == 0) {
            uint16_t eliminate_mask = puzzle->candidates[idx_one];
            if(eliminate_from_intersections(puzzle, eliminate_mask, idx_one, idx_two)) {
              return PROGRESS_MADE;
            }
          }
        }
        uint8_t c1 = IDX_TO_COL[idx_one];
        uint8_t c2 = IDX_TO_COL[idx_two];
        if(v1 == v2 && h1 != h2 && c1 != c2) {
          // check for chute remote pair
          uint8_t unused_col = (v1 * 9 + 3) - c1 - c2; // weird magic math I learned from the internet

          uint8_t h_unused = 3 - IDX_TO_CHUTE_HOR[idx_one] - IDX_TO_CHUTE_HOR[idx_two];

          uint8_t cell_one = (h_unused * 27) + unused_col;
          uint8_t cell_two = cell_one + 9;
          uint8_t cell_three = cell_one + 18;

          uint16_t mask_one = puzzle->cells[cell_one] != 0 ? (1 << (puzzle->cells[cell_one] - 1)) : puzzle->candidates[cell_one];
          uint16_t mask_two = puzzle->cells[cell_two] != 0 ? (1 << (puzzle->cells[cell_two] - 1)) : puzzle->candidates[cell_two];
          uint16_t mask_three = puzzle->cells[cell_three] != 0 ? (1 << (puzzle->cells[cell_three] - 1)) : puzzle->candidates[cell_three];

          uint16_t overlap_mask = puzzle->candidates[idx_one] & (mask_one | mask_two | mask_three);
          int overlap_count = __builtin_popcount(overlap_mask);
          if(overlap_count == 1) {
            uint16_t eliminate_mask = overlap_mask;
            if(eliminate_from_intersections(puzzle, eliminate_mask, idx_one, idx_two)) {
              return PROGRESS_MADE;
            }
          } else if(overlap_count == 0) {
            uint16_t eliminate_mask = puzzle->candidates[idx_one];
            if(eliminate_from_intersections(puzzle, eliminate_mask, idx_one, idx_two)) {
              return PROGRESS_MADE;
            }
          }
        }
      }
    }
  }
  return NO_PROGRESS;
}

bool eliminate_from_intersections(Puzzle* puzzle, uint16_t eliminate_mask, uint8_t idx_one, uint8_t idx_two) {
  uint8_t intersections[20];
  int intersection_count = get_uint8_intersection(CELL_PEERS_LOOKUP[idx_one], CELL_PEERS_LOOKUP[idx_two], 20, 20, intersections);
  bool step_applied = false;
  for(int c = 0; c < intersection_count; ++c) {
    uint8_t idx = intersections[c];
    if(puzzle->candidates[idx] & eliminate_mask) {
      Step step = {
        .eliminated_mask = eliminate_mask,
        .placed_val = 0,
        .target_cell = idx,
        .technique = CHUTE_REMOTE_PAIRS
      };
      apply_step(puzzle, step);
      step_applied = true;
    }
  }
  return step_applied;
}
