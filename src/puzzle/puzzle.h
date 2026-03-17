#ifndef SRC_PUZZLE_PUZZLE_H
#define SRC_PUZZLE_PUZZLE_H

#include "stdint.h"
#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9
#define TOTAL_CELLS 81

typedef enum DifficultyRating {
  BEGINNER,
  EASY,
  MEDIUM,
  HARD,
  IMPOSSIBLE
} DifficultyRating;

typedef struct Puzzle {
  int cells[TOTAL_CELLS];
  uint16_t candidates[TOTAL_CELLS];
} Puzzle;

int parsePuzzleStr(char puzzleStr[], Puzzle* puzzle);
#endif //SRC_PUZZLE_PUZZLE_H
