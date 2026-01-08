#ifndef SRC_FISH_SOLVER_FINNED_FISH_SOLVER_H
#define SRC_FISH_SOLVER_FINNED_FISH_SOLVER_H
#include "puzzle.h"
#include "step.h"
#include "basic_fish_solver.h"

typedef struct {
  int row;
  int col;
} FinLocation;

typedef struct {
  int housesWithCandidateCount;
  FishBases* allFishBases;
  FishBases* potentialBaseCombo;
  uint16_t* allCandidates;
  int candidate;
  bool byRow;
  int* indiciesToRemove;
  int indiciesToRemoveCount;
} FinnedFishContext;

StepNode* findFinnedFish(Puzzle* puzzle, StepNode* head);
StepNode* findFinnedXWing(Puzzle* puzzle, StepNode* head);
StepNode* findFinnedSwordfish(Puzzle* puzzle, StepNode* head);
StepNode* findFinnedJellyfish(Puzzle* puzzle, StepNode* head);
StepNode* findFinnedFishByRow(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head);
StepNode* findFinnedFishByCol(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head);
bool searchFishRecursively(FinnedFishContext* context, int startIndex, int depth, int fishSize);
uint16_t findRowsWithCandidate(uint16_t* puzzleCandidates, int candidate);
uint16_t findColsWithCandidate(uint16_t* puzzleCandidates, int candidate);
void getBlockIndicies(int blockX, int blockY, int* blockIndices);

#endif // SRC_FISH_SOLVER_FINNED_FISH_SOLVER_H