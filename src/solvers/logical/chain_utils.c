#include <stdint.h>
#include "chain_utils.h"
#include "utils.h"
#include "puzzle.h"

void collect_chain_pairs(const Puzzle* puzzle, ChainSearchContext* context, int candidate) {
  SearchParams search_params = {
    .mask = 1 << (candidate - 1),
    .max_count = 2
  };
  int pair_idxs[2];
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    const uint8_t* house_idxs = ROW_TO_IDXS[i];
    int cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      uint8_t cell_one = house_idxs[pair_idxs[0]];
      uint8_t cell_two = house_idxs[pair_idxs[1]];
      context->pairs[context->pairs_count].cell_one = cell_one;
      context->pairs[context->pairs_count++].cell_two = cell_two;
      if(!includes_uint8(context->neighbor_lookup[cell_one], context->neighbor_count[cell_one], cell_two)) {
        context->neighbor_lookup[cell_one][context->neighbor_count[cell_one]++] = cell_two;
        context->neighbor_lookup[cell_two][context->neighbor_count[cell_two]++] = cell_one;
      }

    }
    house_idxs = COL_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      uint8_t cell_one = house_idxs[pair_idxs[0]];
      uint8_t cell_two = house_idxs[pair_idxs[1]];
      context->pairs[context->pairs_count].cell_one = cell_one;
      context->pairs[context->pairs_count++].cell_two = cell_two;
      if(!includes_uint8(context->neighbor_lookup[cell_one], context->neighbor_count[cell_one], cell_two)) {
        context->neighbor_lookup[cell_one][context->neighbor_count[cell_one]++] = cell_two;
        context->neighbor_lookup[cell_two][context->neighbor_count[cell_two]++] = cell_one;
      }
    }
    house_idxs = BLOCK_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      uint8_t cell_one = house_idxs[pair_idxs[0]];
      uint8_t cell_two = house_idxs[pair_idxs[1]];
      context->pairs[context->pairs_count].cell_one = cell_one;
      context->pairs[context->pairs_count++].cell_two = cell_two;
      if(!includes_uint8(context->neighbor_lookup[cell_one], context->neighbor_count[cell_one], cell_two)) {
        context->neighbor_lookup[cell_one][context->neighbor_count[cell_one]++] = cell_two;
        context->neighbor_lookup[cell_two][context->neighbor_count[cell_two]++] = cell_one;
      }
    }
  }
}
