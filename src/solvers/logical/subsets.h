#ifndef SRC_SOLVERS_LOGICAL_SUBSETS_H
#define SRC_SOLVERS_LOGICAL_SUBSETS_H
#include "puzzle.h"
#include "step.h"

TechiqueResult find_naked_pair(Puzzle* puzzle, House* house);
TechiqueResult find_hidden_subset_of_size(Puzzle* puzzle, House* house, int size);


#endif // SRC_SOLVERS_LOGICAL_SUBSETS_H
