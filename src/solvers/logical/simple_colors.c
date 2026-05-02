#include "simple_colors.h"
#include "puzzle.h"

TechniqueResult find_simple_color(Puzzle* puzzle) {
  SimpleColorContext context;
  for(int i = 1; i <= 9; ++i) {
    context.pairs_count = 0;
    collect_chain_pairs(puzzle, &context, i);

    if(context.pairs_count > 2)
  }
}


void collect_chain_pairs(const Puzzle* puzzle, SimpleColorContext* context, int candidate) {
  SearchParams search_params = {
    .mask = 1 << (candidate - 1),
    .max_count = 2
  };
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    const uint8_t* house_idxs = ROW_TO_IDXS[i];
    int cand_count = get_candidate_positions(puzzle, house_idxs, search_params, context->pairs[context->pairs_count].pairIdxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].type = ROW;
    }
    house_idxs = COL_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, context->pairs[context->pairs_count].pairIdxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].type = COL;
    }
    house_idxs = BLOCK_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, context->pairs[context->pairs_count].pairIdxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].type = BLOCK;
    }
  }
}
