#ifndef SRC_MODELS_STRATEGIES_H
#define SRC_MODELS_STRATEGIES_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum {
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
  bool candidateRemoved;
  Strategy strategyUsed;
} Step;

typedef struct StepNode {
  Step step;
  struct Step* next;
  struct Step* prev;
} StepNode;

// Iterates to the end of the list and appends it with a new StepNode with value Step;
StepNode* pushStep(StepNode* head, Step new);
void freeStepList(StepNode* head);
#endif // STRATEGIES_H