#ifndef SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#define SRC_SOLVERS_LOGICAL_SIMPLE_COLORS

typedef struct ChainPair {
  uint8_t cell_one;
  uint8_t cell_two;
} ChainPair;

typedef struct SimpleColorContext {
  ChainPair pairs[40];
  uint8_t chain[40];
  uint8_t colors[81];
  int pairs_count;
} SimpleColorContext;

// start at a node go both ways in the chain marking the colors -> follow the chain to the end 
// check if any of them conflict
#endif
