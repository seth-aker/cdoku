#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "step.h"
#include "utils.h"
#include "logger.h"

StepNode* initStepList() {
   StepNode* newNode = calloc(1, sizeof(StepNode));
   if(newNode == NULL) {
    perror("Failed to allocate memory for new Step");
    exit(EXIT_FAILURE);
  }
  Step emptyStep = {
    .colIndex = -1,
    .rowIndex = -1,
    .value = -1
  };
  newNode->next = NULL;
  newNode->prev = NULL;
  newNode->step = emptyStep;
  return newNode;
}
StepNode* appendStep(StepNode* head, Step newStep) {
  if(head == NULL) {
    head = initStepList();
    head->step = newStep;
    return head;
  }
  StepNode* newNode = calloc(1, sizeof(StepNode));
  if(newNode == NULL) {
    perror("Failed to allocate memory for new Step");
    exit(EXIT_FAILURE);
  }
  StepNode* current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  if(debugModeOn) {
    printStep(newStep, countSteps(head));
  }
  current->next = newNode;
  newNode->step = newStep;
  newNode->prev = current;
  newNode->next = NULL;
  return newNode;
}

int countSteps(StepNode* head) {
  StepNode* current = head;
  int count = 0;
  while(current->prev != NULL) {
    current = current->prev;
  }
  while(current->next != NULL) {
    count++;
    current = current->next;
  }
  return ++count;
}

void getStepSummary(StepSummary* summary, StepNode* head) {
  StepNode* current = head;
  // Get to the real head if we aren't already there.
  while(current->prev != NULL) {
    current = current->prev;
  }
  // Loop through until we get to the end of the list.
  while(current->next != NULL) {
    updateSummary(summary, current->step);
    current = current->next;  
  }
  // Get the last step.
  updateSummary(summary, current->step);
  calculateDifficulty(summary);
}
void updateSummary(StepSummary* summary, Step step) {
  switch (step.strategyUsed) {
  case FULL_HOUSE:
    summary->fullHouseCount++;
    break;
  case NAKED_SINGLE:
    summary->nakedSingleCount++;
    break;
  case HIDDEN_SINGLE:
    summary->hiddenSingleCount++;
    break;
  case LOCKED_CANDIDATE_POINTING:
    summary->lockedCandidatePointingCount++;
    break;
  case LOCKED_CANDIDATE_CLAIMING:
    summary->lockedCandidateClaimingCount++;
    break;
  case HIDDEN_PAIRS:
    summary->hiddenPairsCount++;
    break;
  case NAKED_PAIRS:
    summary->nakedPairsCount++;
    break;
  case HIDDEN_TRIPLES:
    summary->hiddenTriplesCount++;
    break;
  case NAKED_TRIPLES:
    summary->nakedTriplesCount++;
    break;
  case HIDDEN_QUADS:
    summary->hiddenQuadsCount++;
    break;
  case NAKED_QUADS:
    summary->nakedQuadsCount++;
    break;
  case X_WING:
    summary->xWingCount++;
    break;
  case FINNED_X_WING:
    summary->finnedXWingCount++;
    break;
  case SWORDFISH:
    summary->swordfishCount++;
    break;
  case FINNED_SWORDFISH:
    summary->finnedSwordfishCount++;
    break;
  case JELLYFISH:
    summary->jellyfishCount++;
    break;
  case FINNED_JELLYFISH:
    summary->finnedJellyfishCount++;
    break;
  case GUESS:
    summary->guessCount++;
    break;
  default:
    break;
  }
  int stratScore = getStrategyScore(step.strategyUsed);
  if(summary->peakDifficulty < stratScore) {
    summary->peakDifficulty = stratScore;
  }
  summary->totalSteps++;
}

void calculateDifficulty(StepSummary* summary) {
  int workScore = 0;
  summary->difficultyScore = 0;

  if(summary->fullHouseCount) {
    workScore += calcDecayScore(FULL_HOUSE, summary->fullHouseCount);
  }
  if(summary->nakedSingleCount) {
    workScore += calcDecayScore(NAKED_SINGLE, summary->nakedSingleCount);
  }
  if(summary->hiddenSingleCount) {
    workScore += calcDecayScore(HIDDEN_SINGLE, summary->hiddenSingleCount);
  }
  if(summary->lockedCandidatePointingCount) {
    workScore += calcDecayScore(LOCKED_CANDIDATE_POINTING, summary->lockedCandidatePointingCount);
  }
  if(summary->lockedCandidateClaimingCount) {
    workScore += calcDecayScore(LOCKED_CANDIDATE_CLAIMING, summary->lockedCandidateClaimingCount);
  }
  if(summary->hiddenPairsCount) {
    workScore += calcDecayScore(HIDDEN_PAIRS, summary->hiddenPairsCount);
  }
  if(summary->nakedPairsCount) {
    workScore += calcDecayScore(NAKED_PAIRS, summary->nakedPairsCount);
  }
  if(summary->hiddenTriplesCount) {
    workScore += calcDecayScore(HIDDEN_TRIPLES, summary->hiddenTriplesCount);
  }
  if(summary->nakedTriplesCount) {
    workScore += calcDecayScore(NAKED_TRIPLES, summary->nakedTriplesCount);
  }
  if(summary->hiddenQuadsCount) {
    workScore += calcDecayScore(HIDDEN_QUADS, summary->hiddenQuadsCount);
  }
  if(summary->nakedQuadsCount) {
    workScore += calcDecayScore(NAKED_QUADS, summary->nakedQuadsCount);
  }
  if(summary->xWingCount) {
    workScore += calcDecayScore(X_WING, summary->xWingCount);
  }
  if(summary->finnedXWingCount) {
    workScore += calcDecayScore(FINNED_X_WING, summary->finnedXWingCount);
  }
  if(summary->swordfishCount) {
    workScore += calcDecayScore(SWORDFISH, summary->swordfishCount);
  }
  if(summary->finnedSwordfishCount) {
    workScore += calcDecayScore(FINNED_SWORDFISH, summary->finnedSwordfishCount);
  }
  if(summary->jellyfishCount) {
    workScore += calcDecayScore(JELLYFISH, summary->jellyfishCount);
  }
  if(summary->finnedJellyfishCount) {
    workScore += calcDecayScore(FINNED_JELLYFISH, summary->finnedJellyfishCount);
  }
  if(summary->skyscraperCount) {
    workScore += calcDecayScore(SKYSCRAPER, summary->skyscraperCount);
  }
  if(summary->xyWingCount) {
    workScore += calcDecayScore(XY_WING, summary->xyWingCount);
  }
  if(summary->guessCount) {
    workScore += calcDecayScore(GUESS, summary->guessCount);
  }
  
  summary->difficultyScore = summary->peakDifficulty + (int)log10(workScore);
}



void freeStepList(StepNode* head) {
  StepNode* current = head;
  StepNode* next;
  while(current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}