#include "xyz_wing.h"
#include "puzzle.h"
#include "utils.h"
TechniqueResult find_xyz_wing(Puzzle* puzzle) {
  int tri_val_cells[81];
  int tri_cell_count = 0;
  int bi_val_cells[81];
  int bi_cell_count = 0;
  bool progress_made = false;
  for(int i = 0; i < 81; ++i) {
    int cand_count = __builtin_popcount(puzzle->candidates[i]);
    if(cand_count == 3) {
      tri_val_cells[tri_cell_count++] = i;
    } else if(cand_count == 2) {
      bi_val_cells[bi_cell_count++] = i;
    }
  }

  for(int i = 0; i < tri_cell_count; ++i) {
    int pivot_idx = tri_val_cells[i];
    uint16_t pivot_candidates = puzzle->candidates[pivot_idx];

    uint8_t potential_pincers[20];
    int pincer_count = 0;
    // collect potential pincers for pivot
    for(int p = 0; p < bi_cell_count; ++p) {
      if(!IS_PEER_LOOKUP[pivot_idx][bi_val_cells[p]]) {
        continue;
      }
      uint16_t peer_candidates = puzzle->candidates[bi_val_cells[p]];
      if(__builtin_popcount(peer_candidates & pivot_candidates) != 2) {
        continue;
      }
      potential_pincers[pincer_count++] = bi_val_cells[p];
    }

    if(pincer_count <= 1) continue;

    for(int p1 = 0; p1 < pincer_count - 1; ++p1) {
      int p1_idx = potential_pincers[p1];
      uint16_t p1_candidates = puzzle->candidates[p1_idx];
      for(int p2 = p1 + 1; p2 < pincer_count; ++p2) {
        int p2_idx = potential_pincers[p2];
        uint16_t p2_candidates = puzzle->candidates[p2_idx];
        if(IS_PEER_LOOKUP[p1_idx][p2_idx]) {
          continue;
        }
        if(__builtin_popcount(p1_candidates & p2_candidates) != 1) {
          continue;
        }
        const uint8_t* pivot_peers = CELL_PEERS_LOOKUP[pivot_idx];
        uint16_t eliminate_mask = p1_candidates & p2_candidates;

        for(int pi = 0; pi < 20; ++pi) {
          uint8_t idx = pivot_peers[pi];
          if(IS_PEER_LOOKUP[idx][p1_idx] && IS_PEER_LOOKUP[idx][p2_idx]) {
            if(puzzle->candidates[idx] & eliminate_mask) {
              Step step = {
                .eliminated_mask = eliminate_mask,
                .placed_val = 0,
                .target_cell = idx,
                .technique = XYZ_WING
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
