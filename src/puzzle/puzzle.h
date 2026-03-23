#ifndef SRC_PUZZLE_PUZZLE_H
#define SRC_PUZZLE_PUZZLE_H

#include "stdint.h"
#include "stdbool.h"

#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9
#define TOTAL_CELLS 81

typedef enum DiffRating {
  BEGINNER,
  EASY,
  MEDIUM,
  HARD,
  IMPOSSIBLE
} DiffRating;

typedef struct Puzzle {
  uint8_t cells[TOTAL_CELLS];
  uint16_t candidates[TOTAL_CELLS];
} Puzzle;

int findEmptyCell(uint8_t cells[]);
int parsePuzzleStr(char puzzle_str[], Puzzle* puzzle);
bool isValidNumInCell(uint8_t num, int idx, uint8_t cells[]);


extern const uint8_t IDX_TO_ROW[TOTAL_CELLS];
extern const uint8_t IDX_TO_COL[TOTAL_CELLS];
extern const uint8_t IDX_TO_BLOCK[TOTAL_CELLS];
extern const uint8_t CELL_PEERS_LOOKUP[TOTAL_CELLS][20];

#endif //SRC_PUZZLE_PUZZLE_H
