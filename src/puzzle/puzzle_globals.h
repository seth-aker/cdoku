#ifndef SRC_PUZZLE_PUZZLE_GLOBALS_H
#define SRC_PUZZLE_PUZZLE_GLOBALS_H
#include <stdint.h>
#include <stdbool.h>
#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9
#define TOTAL_CELLS 81

typedef struct MaskArr {
  uint8_t length;
  uint8_t candidates[9];
} MaskArr;
extern const uint8_t IDX_TO_ROW[TOTAL_CELLS];
extern const uint8_t IDX_TO_COL[TOTAL_CELLS];
extern const uint8_t IDX_TO_BLOCK[TOTAL_CELLS];
extern const uint8_t BLOCK_IDX_TO_START_COL[9];
extern const uint8_t BLOCK_IDX_TO_START_ROW[9];
extern const uint8_t CELL_PEERS_LOOKUP[TOTAL_CELLS][20];
extern const uint8_t BLOCK_TO_IDXS[PUZZLE_WIDTH][PUZZLE_WIDTH];
extern const uint8_t ROW_TO_IDXS[PUZZLE_WIDTH][PUZZLE_WIDTH];
extern const uint8_t COL_TO_IDXS[PUZZLE_WIDTH][PUZZLE_WIDTH];
extern const uint8_t CELL_POS_IN_BLOCK[PUZZLE_WIDTH][PUZZLE_WIDTH];
extern const MaskArr MASK_TO_ARRAY_LOOKUP[512];
extern const bool IS_PEER_LOOKUP[81][81];
#endif // SRC_PUZZLE_PUZZLE_GLOBALS_H
