#ifndef SRC_SOLVER_SUKOKU_SOLVER_H
#define SRC_SOLVER_SUKOKU_SOLVER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "puzzle.h"
#include "step.h"
#include "basic_fish_solver.h"
#include "finned_fish_solver.h"

typedef struct {
  int* values;
  uint8_t* candidates;
  int* subsetIndicies;
  int* foundIndicies;
  uint8_t subsetCandidates;
} NakedComboSearchContext;

typedef struct {
  int* emptyCellIndicies;
  int emptyCellCount;
  int* cells;
  uint8_t* candidates;
  uint8_t* candidateSubset;
  int* allCandidateArray;
  int allCandidateCount;
  int* hiddenComboCellIndices;
} HiddenComboSearchContext;

bool solvePuzzle(Puzzle* puzzle, StepNode* head);
bool solveRecursive(Puzzle* puzzle, StepNode* head);

bool isPuzzleSolved(int* cells);

StepNode* findSingles(Puzzle* puzzle, StepNode* head);
StepNode* findFullHouses(Puzzle* puzzle, StepNode* head);
StepNode* findNakedSingles(Puzzle* puzzle, StepNode* head);
StepNode* findHiddenSingles(Puzzle* puzzle, StepNode* head);

StepNode* findLockedCandidates(Puzzle* puzzle, StepNode* head);
StepNode* findLockedCandidatePointing(Puzzle* puzzle, StepNode* head);
StepNode* findLockedCandidateClaiming(Puzzle* puzzle, StepNode* head);

StepNode* findSubsets(Puzzle* puzzle, StepNode* head);
StepNode* findNakedSubsetOfSize(Puzzle* puzzle, House* house, int subsetSize, StepNode* head);
StepNode* removeNakedSubsetFromHouse(Puzzle* puzzle, NakedComboSearchContext* context, House* house, int subsetSize, StepNode* head);
bool findNakedCombo(NakedComboSearchContext* context, int startIndex, int subsetSize, int depth);

StepNode* findHiddenSubsetOfSize(Puzzle* puzzle, House* house, int subsetSize, StepNode* head);
StepNode* removeHiddenSubsetFromHouse(Puzzle* puzzle, House* house, HiddenComboSearchContext* context, int subsetSize, StepNode* head);
bool findHiddenCombo(HiddenComboSearchContext* context, int startIndex, int subsetSize, int depth);

StepNode* removePointingRow(int rowIndex, int skipBlockCol, uint8_t valuesToRemove, Puzzle* puzzle, StepNode* head);
StepNode* removePointingCol(int colIndex, int skipBlockRow, uint8_t valuesToRemove, Puzzle* puzzle, StepNode* head);

bool isFullHouse(int cellIndex, int* cells);

StepNode* removeCandidateFromRow(int rowIndex, int value, Puzzle* puzzle, StepNode* head);
StepNode* removeCandidateFromCol(int colIndex, int value, Puzzle* puzzle, StepNode* head);
StepNode* applyFoundValue(Puzzle* puzzle, int value, int cellIndex, Strategy stratUsed, StepNode* head);
bool makeGuess(Puzzle* puzzle, StepNode* head);

#endif // SUKOKU_SOLVER_H
