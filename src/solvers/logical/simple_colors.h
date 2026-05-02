#ifndef SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#define SRC_SOLVERS_LOGICAL_SIMPLE_COLORS

typedef enum HouseType {
  ROW,
  COL,
  BOX
} HouseType;

typedef struct ChainPair {
  int pairIdxs[2];
  HouseType type;
} ChainPair;

typedef struct SimpleColorContext {
  ChainPair pairs[27];
  int pairs_count;
} SimpleColorContext;
#endif
