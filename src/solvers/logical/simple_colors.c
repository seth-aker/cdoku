#include "simple_colors.h"
#include "puzzle.h"

TechniqueResult find_simple_color(Puzzle* puzzle) {
  SimpleColorContext context;
  for(int i = 1; i <= 9; ++i) {
    context.pairs_count = 0;
    collect_chain_pairs(puzzle, &context, i);

    if(context.pairs_count < 2) {
      continue;
    }
    for(int p = 0; p < context.pairs_count; ++p) {


    }
  }
}


void collect_chain_pairs(const Puzzle* puzzle, SimpleColorContext* context, int candidate) {
  SearchParams search_params = {
    .mask = 1 << (candidate - 1),
    .max_count = 2
  };
  int pair_idxs[2];
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    const uint8_t* house_idxs = ROW_TO_IDXS[i];
    int cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      int pair_count = context->pairs_count;
      ChainCell* cell_one = &context->pairs[pair_count].pair[0];
      ChainCell* cell_two = &context->pairs[pair_count].pair[1];
      cell_one->idx = pair_idxs[0];
      cell_two->idx = pair_idxs[1];
      cell_one->row = i;
      cell_two->row = i;
      cell_one->col = IDX_TO_COL[pair_idxs[0]];
      cell_two->col = IDX_TO_COL[pair_idxs[1]];
      // add the cells to the row set
      context->rowCells[i].cells[context->rowCells[i].count++] = &cell_one;
      context->rowCells[i].cells[context->rowCells[i].count++] = &cell_two;
      // add the cells to the col set

      context->pairs_count++;
    }
    house_idxs = COL_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].type = COL;
    }
    house_idxs = BLOCK_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].type = BLOCK;
    }
  }
}
