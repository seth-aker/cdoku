#ifndef SRC_SOLVER_SUKOKU_SOLVER_H
#define SRC_SOLVER_SUKOKU_SOLVER_H

#include <stdlib.h>;
#include <stdbool.h>;
#include "puzzle.h"


#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9

bool solvePuzzle(Puzzle* puzzle, StepNode* steps);

void fillPuzzleCandidates(Puzzle* Puzzle);
bool isPuzzleSolved(Cells* cells);
bool numWorksInCell(int rowIndex, int colIndex, int potentialNum, Cells* cells);

bool findSingle(Puzzle* puzzle, Step* step);
int findHiddenSingle(int rowIndex, int colIndex, Puzzle* puzzle);
bool isFullHouse(int rowIndex, int colIndex, Cells* cells);


void getBlock(int blockX, int blockY, Cells* cells, int* block);
void getRow(int rowIndex, Cells* cells, int* row);
void getCol(int colIndex, Cells* cells, int* col);
void getCandidateBlock(int blockX, int blockY, CellCandidates* candidates, CellCandidates* block);
void getCandidateRow(int rowIndex, CellCandidates* candidates, CellCandidates* row);
void getCandidateCol(int colIndex, CellCandidates* candidates, CellCandidates* col);
int getCellPosInBlock(int rowIndex, int colIndex);

static inline bool hasCandidate(CellCandidates mask, int num) {
  return (mask >> (num - 1)) & 1;
}
static inline void addCandidate(CellCandidates* mask, int num) {
  *mask |= (1 << (num - 1));
}
static inline bool removeCandidate(CellCandidates* mask, int num) {
  bool removed = hasCandidate(*mask, num);
  *mask &= ~(1 << (num - 1));
  return removed;
}
static inline int getValueFromMask(uint16_t mask) {
  if(mask == 0) return 0;
  return __builtin_ctz(mask) + 1;
}
static inline int countUnit(int* unit) {
  int count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(unit[i] != 0) {
      ++count;
    }
  }
  return count;
}
#endif // SUKOKU_SOLVER
