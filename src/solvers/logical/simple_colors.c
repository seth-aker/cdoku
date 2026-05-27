#include <string.h>
#include "simple_colors.h"
#include "puzzle.h"
#include "utils.h"
TechniqueResult find_simple_color(Puzzle* puzzle) {
  SimpleColorContext context;
  for(int i = 1; i <= 9; ++i) {
    uint16_t candidate_mask = 1 << (i - 1);
    memset(context.neighbor_count, 0, sizeof(context.neighbor_count));
    context.pairs_count = 0;
    collect_chain_pairs(puzzle, &context, i);

    if(context.pairs_count < 2) {
      continue;
    }
    bool visited[81] = { false };
    for(int pair_idx = 0; pair_idx < context.pairs_count; ++pair_idx) {
      uint8_t start_cell = context.pairs[pair_idx].cell_one;
      if(visited[start_cell]) {
        continue;
      }
      context.chain_len = paint_colors(&context, start_cell);

      for(int c = 0; c < context.chain_len; ++c) {
        uint8_t cell_one = context.chain[c];
        uint8_t cell_one_color = context.colors[cell_one];
        visited[cell_one] = true;

        for(int n = 0; n < context.neighbor_count[cell_one]; ++n) {
          uint8_t neighbor = context.neighbor_lookup[cell_one][n];
          uint8_t neighbor_color = context.colors[neighbor];
          if(neighbor_color == cell_one_color) {
            apply_val_to_chain_color(puzzle, &context, 1 - cell_one_color, i);
            return PROGRESS_MADE;
          }
        }
      }

      for(int cell_idx = 0; cell_idx < 81; ++cell_idx) {
        // if cell is empty, not colored, and has the target candidate
        if(puzzle->cells[cell_idx] == 0 && context.colors[cell_idx] == -1 && puzzle->candidates[cell_idx] & candidate_mask) {
          const uint8_t* peers = CELL_PEERS_LOOKUP[cell_idx];
          bool seen_color_zero = false;
          bool seen_color_one = false;
          for(int p = 0; p < 20; ++p) {
            if(context.colors[peers[p]] == 0) {
              seen_color_zero = true;
            } else if(context.colors[peers[p]] == 1) {
              seen_color_one = true;
            }

            if(seen_color_zero && seen_color_one) {
              Step step = {
                .eliminated_mask = candidate_mask,
                .placed_val = 0,
                .target_cell = cell_idx,
                .technique = SIMPLE_COLORS
              };
              apply_step(puzzle, step);
              return PROGRESS_MADE;
            }
          }
        }
      }
    }
  }
  return NO_PROGRESS;
}

int paint_colors(SimpleColorContext* context, uint8_t start_cell) {
  int head = 0, tail = 0;
  memset(context->colors, -1, sizeof(int8_t) * 81);
  context->chain[tail++] = start_cell;
  context->colors[start_cell] = 0;

  while(head < tail) {
    uint8_t current_cell = context->chain[head++];
    uint8_t current_color = context->colors[current_cell];
    uint8_t next_color = 1 - current_color; // alternate between 0 and 1

    for(int i = 0; i < context->neighbor_count[current_cell]; ++i) {
      uint8_t neighbor = context->neighbor_lookup[current_cell][i];
      if(context->colors[neighbor] == -1) {
        context->colors[neighbor] = next_color;
        context->chain[tail++] = neighbor;
      }
    }
  }
  return tail;
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

void apply_val_to_chain_color(Puzzle* puzzle, SimpleColorContext* context, uint8_t color, uint8_t value) {
  for(int i = 0; i < context->chain_len; ++i) {
    uint8_t cell_color = context->colors[context->chain[i]];
    if(cell_color == color) {
      Step step = {
        .placed_val = value,
        .eliminated_mask = puzzle->candidates[context->chain[i]],
        .target_cell = context->chain[i],
        .technique = SIMPLE_COLORS
      };
      apply_step(puzzle, step);
    }
  }
}
