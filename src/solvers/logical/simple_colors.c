#include <string.h>
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
    int head = 0, tail = 0;
    memset(context.colors, -1, sizeof(uint8_t) * 81);
    uint8_t start_cell = context.pairs[0].cell_one;
    context.chain[tail++] = start_cell;
    context.colors[start_cell] = 0;

    while(head < tail) {
      uint8_t current_cell = context.chain[head++];
      uint8_t current_color = context.colors[current_cell];
      uint8_t next_color = 1 - current_color;

      // in the context of these chains, there are not too many. This search is inefficient, however the smalls size allows for faster lookup in cpu cache
      for(int i = 0; i < context.pairs_count; ++i) {
        uint8_t neighbor = 81;
        if(context.pairs[i].cell_one == current_cell) {
          neighbor = context.pairs[i].cell_two;
        }
        if(context.pairs[i].cell_two == current_cell) {
          neighbor = context.pairs[i].cell_one;
        }

        if(neighbor != 81 && context.colors[neighbor] == -1) {
          context.colors[neighbor] = next_color;
          context.chain[tail++] = neighbor;
        }
      }
    }



  }
}

void paint_colors(SimpleColorContext* context, uint8_t start_cell) {

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
      context->pairs[context->pairs_count++].cell_one = (uint8_t)pair_idxs[0];
      context->pairs[context->pairs_count++].cell_two = (uint8_t)pair_idxs[1];
    }
    house_idxs = COL_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].cell_one = (uint8_t)pair_idxs[0];
      context->pairs[context->pairs_count++].cell_two = (uint8_t)pair_idxs[1];
    }
    house_idxs = BLOCK_TO_IDXS[i];
    cand_count = get_candidate_positions(puzzle, house_idxs, search_params, pair_idxs);
    if(cand_count == 2) {
      context->pairs[context->pairs_count++].cell_one = (uint8_t)pair_idxs[0];
      context->pairs[context->pairs_count++].cell_two = (uint8_t)pair_idxs[1];
    }
  }
}
void collect_neighbors(const Puzzle* puzzle, SimpleColorContext* context, SearchParams search_params) {
  
}