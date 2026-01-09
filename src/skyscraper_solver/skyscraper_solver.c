#include "skyscraper_solver.h"

StepNode* findSkyscrapers(Puzzle* puzzle, StepNode* head) {
  SkyscraperContext context = {0};

  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    context.candidateMask = 1 << (1 - candidate);
    context.candidate = candidate;
  }
}

StepNode* findSkyscraperByRow(Puzzle* puzzle, SkyscraperContext* context, StepNode* head) {
  for(int i = 0; i < PUZZLE_WIDTH - 1; ++i) {
    context->house1Index = i;
    for(int k = i + 1; k < PUZZLE_WIDTH; ++k) {
      context->house2Index = k;
    }
  }
}

