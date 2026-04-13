#ifndef SRC_SOLVERS_LOGICAL_X_WING_H
#define SRC_SOLVERS_LOGICAL_X_WING_H
#include "step.h"
#include "puzzle.h"

typedef struct XWing_Context {
  int cover_houses[9][2];
  int base_house_idx[9];
  int base_house_count;
} XWing_Context;

TechniqueResult find_x_wing(Puzzle* puzzle);
#endif
