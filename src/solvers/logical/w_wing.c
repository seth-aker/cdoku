#include <stdbool.h>
#include <string.h>
#include "w_wing.h"
#include "puzzle.h"
#include "chain_utils.h"
#include "utils.h"
TechniqueResult find_w_wings(Puzzle* puzzle) {
  uint8_t bi_value_bins[512][18];
  uint8_t bin_count[512] = { 0 };
  collect_bi_value_pairs(puzzle, bi_value_bins, bin_count);

  for(int mask = 1; mask < 512; ++mask) {
    if(bin_count[mask] < 2) continue;

    int c1_idx = __builtin_ctz(mask);
    uint16_t cand_mask_1 = 1 << c1_idx;
    uint16_t cand_mask_2 = mask & ~cand_mask_1;

    int cand_one_val = c1_idx + 1;
    int cand_two_val = __builtin_ctz(cand_mask_2) + 1;
    Array_UInt8 bin = {
     .arr = bi_value_bins[mask],
     .count = bin_count[mask]
    };
    if(check_w_wing_link(puzzle, &bin, cand_one_val, cand_mask_2)) {
      return PROGRESS_MADE;
    }
    if(check_w_wing_link(puzzle, &bin, cand_two_val, cand_mask_1)) {
      return PROGRESS_MADE;
    }
  }
  return NO_PROGRESS;

}
bool check_w_wing_link(Puzzle* puzzle, Array_UInt8* bin, int link_val, uint16_t eliminate_mask) {
  ChainSearchContext context;
  memset(context.neighbor_count, 0, sizeof(context.neighbor_count));
  context.pairs_count = 0;

  collect_chain_pairs(puzzle, &context, link_val);

  for(int i = 0; i < bin->count - 1; ++i) {
    for(int j = i + 1; j < bin->count; ++j) {
      uint8_t idx_one = bin->arr[i];
      uint8_t idx_two = bin->arr[j];
      if(IS_PEER_LOOKUP[idx_one][idx_two]) continue;

      for(int p = 0; p < context.pairs_count; ++p) {
        uint8_t sl1 = context.pairs[p].cell_one;
        uint8_t sl2 = context.pairs[p].cell_two;

        if(sl1 == idx_one || sl1 == idx_two || sl2 == idx_one || sl2 == idx_two) {
          continue;
        }

        if((IS_PEER_LOOKUP[idx_one][sl1] && IS_PEER_LOOKUP[idx_two][sl2]) ||
          (IS_PEER_LOOKUP[idx_two][sl1] && IS_PEER_LOOKUP[idx_one][sl2])) {
          if(eliminate_from_intersections(puzzle, eliminate_mask, idx_one, idx_two)) {
            return true;
          }
        }
      }

    }
  }
  return false;
}
