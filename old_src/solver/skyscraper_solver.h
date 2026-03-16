#ifndef SRC_SOLVER_SKYSCRAPER_SOLVER_H
#define SRC_SOLVER_SKYSCRAPER_SOLVER_H
#include "step.h"
#include "puzzle.h"
typedef struct {
  int house1Index;
  int house2Index;
  int candidate;
  uint16_t candidateMask;

} SkyscraperContext;

StepNode* findSkyscrapers(Puzzle* puzzle, StepNode* head);


#endif // SRC_SOLVER_SKYSCRAPER_SOLVER_H
