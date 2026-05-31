#include "xy_wing.h"
#include "utils.h"
#include <stdbool.h>

TechniqueResult find_xy_wings(Puzzle* puzzle) {
  int bi_value_cells[81];
  int bi_val_cell_count = 0;
  bool progress_made = false;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(__builtin_popcount(puzzle->candidates[i]) == 2) {
      bi_value_cells[bi_val_cell_count++] = i;
    }
  }
  for(int i = 0; i < bi_val_cell_count; ++i) {
    int pivot_idx = bi_value_cells[i];
    uint16_t pivot_candidates = puzzle->candidates[pivot_idx];

    int potential_pincers[20];
    int potential_pincer_count = 0;
    for(int p = 0; p < bi_val_cell_count; ++p) {
      if(p == i) {
        continue;
      }
      if(!IS_PEER_LOOKUP[pivot_idx][bi_value_cells[p]]) {
        continue;
      }

      uint16_t peer_candidates = puzzle->candidates[bi_value_cells[p]];
      if(__builtin_popcount(peer_candidates & pivot_candidates) != 1) {
        continue;
      }
      potential_pincers[potential_pincer_count++] = bi_value_cells[p];
    }
    if(potential_pincer_count <= 1) {
      continue;
    }

    for(int p1 = 0; p1 < potential_pincer_count - 1; ++p1) {
      int p1_idx = potential_pincers[p1];
      uint16_t p1_candidates = puzzle->candidates[p1_idx];
      for(int p2 = p1 + 1; p2 < potential_pincer_count; ++p2) {
        uint8_t p2_idx = potential_pincers[p2];
        if(IS_PEER_LOOKUP[p1_idx][p2_idx]) {
          continue;
        }
        uint16_t p2_candidates = puzzle->candidates[p2_idx];
        if(__builtin_popcount(pivot_candidates | p1_candidates | p2_candidates) != 3) {
          continue;
        }

        const uint8_t* p1_peers = CELL_PEERS_LOOKUP[p1_idx];
        uint16_t eliminate_mask = p1_candidates & p2_candidates;
        for(int pi = 0; pi < 20; ++pi) {
          uint8_t idx = p1_peers[pi];
          if(idx == pivot_idx) {
            continue;
          }
          if(IS_PEER_LOOKUP[idx][p2_idx]) {
            if(puzzle->candidates[idx] & eliminate_mask) {
              Step step = {
                .eliminated_mask = eliminate_mask,
                .placed_val = 0,
                .target_cell = idx,
                .technique = XY_WING
              };
              apply_step(puzzle, step);
              progress_made = true;
            }
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
