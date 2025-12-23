#ifndef SRC_SOLVER_PUZZLE_H
#define SRC_SOLVER_PUZZLE_H

#include <stdint.h>
#include <stdbool.h>

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
  int blockX;
  int blockY;
} BlockCoord;

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


void fillPuzzleCandidates(Puzzle* Puzzle);

void getBlock(int blockX, int blockY, int* cells, int* block);
void getRow(int rowIndex, int* cells, int* row);
void getCol(int colIndex, int* cells, int* col);
int getCellPosInBlock(int rowIndex, int colIndex);

int getCellIndexFromHousePos(House* house, int cellIndex);
void getCandidateBlock(int blockX, int blockY, uint16_t* candidates, uint16_t* block);
void getCandidateRow(int rowIndex, uint16_t* candidates, uint16_t* row);
void getCandidateCol(int colIndex, uint16_t* candidates, uint16_t* col);

int getCandidatesInCell(uint16_t cellCandidates, int* candidateArray);
bool numWorksInCell(int rowIndex, int colIndex, int potentialNum, int* cells);
int findEmptyCell(Puzzle* puzzle);

static inline bool hasCandidate(uint16_t mask, int num) {
  return (mask >> (num - 1)) & 1;
}
static inline void addCandidate(uint16_t* mask, int num) {
  *mask |= (1 << (num - 1));
}
static inline bool removeCandidate(uint16_t* mask, int num) {
  bool removed = hasCandidate(*mask, num);
  *mask &= ~(1 << (num - 1));
  return removed;
}
static inline int getFirstValueFromMask(uint16_t mask) {
  if(mask == 0) return 0;
  return __builtin_ctz(mask) + 1;
}
static inline int countFilledCells(int* unit) {
  int count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(unit[i] != 0) {
      ++count;
    }
  }
  return count;
}
#endif // SRC_SOLVER_PUZZLE