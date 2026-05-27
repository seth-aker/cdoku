#ifndef SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#define SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#include "stdint.h"
#include "puzzle.h"
#include "step.h"
typedef struct ChainPair {
  uint8_t cell_one;
  uint8_t cell_two;
} ChainPair;

typedef struct SimpleColorContext {
  uint8_t neighbor_count[81];
  uint8_t neighbor_lookup[81][3];
  ChainPair pairs[40];
  uint8_t chain[40];
  int8_t colors[81];
  int chain_len;
  int pairs_count;
} SimpleColorContext;

TechniqueResult find_simple_color(Puzzle* puzzle);

int paint_colors(SimpleColorContext* context, uint8_t start_cell);

void collect_chain_pairs(const Puzzle* puzzle, SimpleColorContext* context, int candidate);

void apply_val_to_chain_color(Puzzle* puzzle, SimpleColorContext* context, uint8_t color, uint8_t value);

#endif
