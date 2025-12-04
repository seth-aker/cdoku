#ifndef SRC_SOLVER_SUKOKU_SOLVER_H
#define SRC_SOLVER_SUKOKU_SOLVER_H

#include <stdlib.h>
#include <stdbool.h>
#include "puzzle.h"
#include "strategies.h"
#include "utils.h"

#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9

typedef struct {
  int* values;
  uint16_t* candidates;
  int* subsetIndicies;
  int* foundIndicies;
  uint16_t subsetCandidates;
} NakedComboSearchContext;

typedef struct {
  int* emptyCellIndicies;
  int emptyCellCount;
  int* cells;
  uint16_t* candidates;
  int* subsetIndicies;
} HiddenComboSearchContext;

bool solvePuzzle(Puzzle* puzzle, StepNode* head);

void fillPuzzleCandidates(Puzzle* Puzzle);
bool isPuzzleSolved(int* cells);

StepNode* findSingle(Puzzle* puzzle, StepNode* head);
int findHiddenSingle(int rowIndex, int colIndex, Puzzle* puzzle);
StepNode* findLockedCandidates(Puzzle* puzzle, StepNode* head);
StepNode* findLockedCandidatePointing(Puzzle* puzzle, StepNode* head);
StepNode* findLockedCandidateClaiming(Puzzle* puzzle, StepNode* head);

StepNode* findSubsets(Puzzle* puzzle, StepNode* head);
StepNode* findNakedSubsetOfSize(Puzzle* puzzle, House* house, int subsetSize, StepNode* head);

StepNode* removePointingRow(int rowIndex, int skipBlockCol, int valueToRemove, Puzzle* puzzle, StepNode* head);
StepNode* removePointingCol(int colIndex, int skipBlockRow, int valueToRemove, Puzzle* puzzle, StepNode* head);

bool numWorksInCell(int rowIndex, int colIndex, int potentialNum, int* cells);
void getBlock(int blockX, int blockY, int* cells, int* block);
void getRow(int rowIndex, int* cells, int* row);
void getCol(int colIndex, int* cells, int* col);
int getCellPosInBlock(int rowIndex, int colIndex);

int getCellIndexFromHousePos(House* house, int cellIndex);
void getCandidateBlock(int blockX, int blockY, uint16_t* candidates, uint16_t* block);
void getCandidateRow(int rowIndex, uint16_t* candidates, uint16_t* row);
void getCandidateCol(int colIndex, uint16_t* candidates, uint16_t* col);

bool isFullHouse(int rowIndex, int colIndex, int* cells);
int getCandidatesInCell(uint16_t cellCandidates, int* candidateArray);

StepNode* removeCandidateFromRow(int rowIndex, int value, Puzzle* puzzle, StepNode* head);
StepNode* removeCandidateFromCol(int colIndex, int value, Puzzle* puzzle, StepNode* head);
StepNode* removeCandidateFromBlock(int blockX, int blockY, int value, int skipRow, int skipCol, Puzzle* puzzle, StepNode* head);

StepNode* removeNakedSubsetFromHouse(Puzzle* puzzle, NakedComboSearchContext* context, House* house, int subsetSize, StepNode* head);
bool findNakedCombo(NakedComboSearchContext* context, int startIndex, int subsetSize, int depth);
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
static inline int getValueFromMask(uint16_t mask) {
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

#endif // SUKOKU_SOLVER
