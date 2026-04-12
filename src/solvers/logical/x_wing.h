#ifndef SRC_SOLVERS_LOGICAL_X_WING_H
#define SRC_SOLVERS_LOGICAL_X_WING_H
#include "step.h"
#include "puzzle.h"

typedef struct XWing_Context {
  int cover_cols[9][2];
  int base_house_idx[9];
  int base_house_count;
  uint16_t candidate_mask;
} XWing_Context;

TechniqueResult find_x_wing(Puzzle* puzzle);
int count_candidate_in_house(Puzzle* puzzle, const uint8_t house_candidates[], XWing_Context* search_context, int max_count);
#endif
