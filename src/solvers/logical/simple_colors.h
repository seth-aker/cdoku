#ifndef SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#define SRC_SOLVERS_LOGICAL_SIMPLE_COLORS

typedef enum HouseType {
  ROW,
  COL,
  BOX
} HouseType;

typedef enum Color {
  DARK,
  LIGHT
} Color;
typedef struct ChainHouse {
  ChainCell* cells[9];
  int count;
} ChainHouse;

typedef struct ChainCell {
  int idx;
  int row;
  int col;
  Color color;
} ChainCell;

typedef struct ChainPair {
  ChainCell pair[2];
  HouseType type;
} ChainPair;

typedef struct SimpleColorContext {
  ChainPair pairs[27];
  ChainHouse rowCells[9];
  ChainHouse colCells[9];
  ChainHouse blockCells[9];
  int pairs_count;
} SimpleColorContext;

// start at a node go both ways in the chain marking the colors -> follow the chain to the end 
// check if any of them conflict
#endif
