#ifndef SRC_FISH_SOLVER_BASIC_FISH_SOLVER_H
#define SRC_FISH_SOLVER_BASIC_FISH_SOLVER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "puzzle.h"
#include "step.h"
#include "utils.h"

typedef struct {
  uint16_t indicies;
  int baseIndex;
} FishIndicies;

typedef struct {
  FishIndicies bases[4]; // Max size of fish is 4.
  int numOfBases;
} FishBases;

StepNode* findBasicFish(Puzzle* puzzle, StepNode* head);
StepNode* findBasicXWing(Puzzle* puzzle, StepNode* head);
StepNode* findBasicSwordfish(Puzzle* puzzle, StepNode* head);
StepNode* findBasicJellyfish(Puzzle* puzzle, StepNode* head);

StepNode* findBasicFishByRow(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head);
StepNode* findBasicFishByCol(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head);
void findCandidateIndiciesInHouse(uint16_t* house, uint16_t candidateMask, uint16_t* indicies, int indiciesSize);
int countCandidateInHouse(uint16_t* house, uint16_t candidateMask, int maxCount);
StepNode* removeCandidatesFromColExcept(Puzzle* puzzle, uint16_t candidateMask, int colIndex, int* exceptIndicies, int exceptSize, Strategy stratUsed, StepNode* head);
StepNode* removeCandidatesFromRowExcept(Puzzle* puzzle, uint16_t candidateMask, int rowIndex, int* exceptIndicies, int exceptSize, Strategy stratUsed, StepNode* head);

#endif // SRC_FISH_SOLVER_BASIC_FISH_SOLVER_H