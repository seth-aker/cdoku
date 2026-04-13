#ifndef SRC_SOLVERS_LOGICAL_SKYSCRAPER_H
#define SRC_SOLVERS_LOGICAL_SKYSCRAPER_H
#include "puzzle.h"
#include "step.h"

typedef struct SkyscraperContext {
  int cover_houses[9][2];
  int base_house_idxs[9];
  int base_house_count;
} SkyscraperContext;

TechniqueResult find_skyscrapers(Puzzle* puzzle);

#endif // SRC_SOLVERS_LOGICAL_SKYSCRAPER_H
