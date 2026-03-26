#ifndef SRC_SOLVERS_LOGICAL_SINGLES_H
#define SRC_SOLVERS_LOGICAL_SINGLES_H

#include <stdbool.h>
#include "puzzle.h"
#include "step.h"

TechiqueResult find_naked_single(Puzzle* puzzle);
bool is_full_house(Puzzle* puzzle, uint8_t idx);
TechiqueResult find_hidden_single(Puzzle* puzzle);

#endif //  SRC_SOLVERS_LOGICAL_SINGLES_H
