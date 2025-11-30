#ifndef SRC_MODELS_PUZZLE_H
#define SRC_MODELS_PUZZLE_H

#include <stdint.h>;
#include "strategies.h";

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

typedef uint16_t CellCandidates;

#define FULL_SET 0X1FF;
#define EMPTY_SET 0x0;

typedef int Cells[81];

typedef struct {
  int id;
  Cells cells;
  CellCandidates candidates[81];
  PuzzleDifficulty difficulty;
} Puzzle;

#endif //PUZZLE