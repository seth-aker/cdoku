#ifndef SRC_FISH_SOLVER_FINNED_FISH_SOLVER_H
#define SRC_FISH_SOLVER_FINNED_FISH_SOLVER_H
#include "puzzle.h"
#include "step.h"
#include "basic_fish_solver.h"

typedef struct {
  int houseWithCandidateCount;
  int* indiciesWithCandidate;
  FishBases* potenialFishBases;
  uint16_t* allCandidates;
  int candidate;
} FinnedFishContext;
#endif // SRC_FISH_SOLVER_FINNED_FISH_SOLVER_H