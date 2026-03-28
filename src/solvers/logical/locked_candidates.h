#ifndef SRC_SOLVERS_LOGICAL_LOCKED_CANDIDATES_H
#define SRC_SOLVERS_LOGICAL_LOCKED_CANDIDATES_H
#include "puzzle.h"
#include "step.h"

TechiqueResult find_locked_candidate_pointing(Puzzle* puzzle);
TechiqueResult find_locked_candidate_claiming(Puzzle* puzzle);

#endif // SRC_SOLVERS_LOGICAL_LOCKED_CANDIDATES_H
