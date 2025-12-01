#ifndef SRC_SOLVER_PUZZLE_H
#define SRC_SOLVER_PUZZLE_H

#include <stdint.h>
#include "strategies.h"

typedef enum {
  BEGINNER,
  EASY,
  MEDIUM,
  HARD,
  IMPOSSIBLE
} DifficultyRating;

typedef struct {
  int score;
  DifficultyRating rating;
} PuzzleDifficulty;


#define FULL_SET 0X1FF;
#define EMPTY_SET 0x0;


typedef struct {
  int id;
  int cells[81];
  uint16_t candidates[81];
  PuzzleDifficulty difficulty;
} Puzzle;

#endif // SRC_SOLVER_PUZZLE