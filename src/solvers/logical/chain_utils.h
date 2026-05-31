#ifndef SRC_SOLVERS_LOGICAL_CHAIN_UTILS_H
#define SRC_SOLVERS_LOGICAL_CHAIN_UTILS_H
#include <stdint.h>
#include "puzzle.h"
typedef struct ChainPair {
  uint8_t cell_one;
  uint8_t cell_two;
} ChainPair;

typedef struct ChainSearchContext {
  uint8_t neighbor_count[81];
  uint8_t neighbor_lookup[81][3];
  ChainPair pairs[40];
  uint8_t chain[40];
  int8_t colors[81];
  int chain_len;
  int pairs_count;
} ChainSearchContext;

void collect_chain_pairs(const Puzzle* puzzle, ChainSearchContext* context, int candidate);

#endif // SRC_SOLVERS_LOGICAL_CHAIN_UTILS_H
