#ifndef SRC_SOLVER_PUZZLE_H
#define SRC_SOLVER_PUZZLE_H

#include <stdint.h>

#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9

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



typedef struct {
  int id;
  int cells[81];
  uint16_t candidates[81];
  PuzzleDifficulty difficulty;
} Puzzle;
typedef enum {
  BLOCK,
  ROW,
  COL
} HouseType;

typedef struct {
  int cells[9];
  uint16_t candidates[9];
  HouseType type;
  int index;
} House;
#endif // SRC_SOLVER_PUZZLE