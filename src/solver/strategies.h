#ifndef SRC_SOLVER_STRATEGIES_H
#define SRC_SOLVER_STRATEGIES_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum {
  NONE,
  FULL_HOUSE,
  NAKED_SINGLE,
  HIDDEN_SINGLE,
  LOCKED_CANDIDATE_POINTING,
  LOCKED_CANDIDATE_CLAIMING,
  HIDDEN_PAIRS,
  NAKED_PAIRS,
  HIDDEN_TRIPLES,
  NAKED_TRIPLES,
  HIDDEN_QUADS,
  NAKED_QUADS,
  GUESS
} Strategy;

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

StepNode* initStepList();
// Iterates to the end of the list and appends it with a new StepNode with value Step;
StepNode* appendStep(StepNode* head, Step newStep);
void freeStepList(StepNode* head);
#endif // SRC_SOLVER_STRATEGIES_H