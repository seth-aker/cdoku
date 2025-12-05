#ifndef SRC_SOLVER_STEP_H
#define SRC_SOLVER_STEP_H
#include <stdint.h>
#include <stdlib.h>
#include "strategies.h"
typedef struct Step {
  int rowIndex;
  int colIndex;
  int value;
  uint16_t candidatesRemoved;
  Strategy strategyUsed;
} Step;

typedef struct StepNode {
  Step step;
  struct StepNode* next;
  struct StepNode* prev;
} StepNode;

typedef struct StepSummary {
  int totalSteps;
  bool solved;
  int fullHouseCount;
  int nakedSingleCount;
  int hiddenSingleCount;
  int lockedCandidatePointingCount;
  int lockedCandidateClaimingCount;
  int hiddenPairsCount;
  int nakedPairsCount;
  int hiddenTriplesCount;
  int nakedTriplesCount;
  int hiddenQuadsCount;
  int nakedQuadsCount;
  int guessCount;
  
} StepSummary;

StepNode* initStepList();
// Iterates to the end of the list and appends it with a new StepNode with value Step;
StepNode* appendStep(StepNode* head, Step newStep);

// Populates StepSummary
void getStepSummary(StepSummary* summary, StepNode* head);
// Only updates counts, not solved.
void updateSummary(StepSummary* summary, Step step);

void freeStepList(StepNode* head);

#endif // SRC_SOLVER_STEP_H