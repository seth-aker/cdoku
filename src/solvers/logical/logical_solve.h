#ifndef SRC_SOLVERS_LOGICAL_LOGICAL_SOLVE_H
#define SRC_SOLVERS_LOGICAL_LOGICAL_SOLVE_H
#include "puzzle.h"
#include "step.h"
#define CHECK_PROGRESS(progress) if(progress == PROGRESS_MADE) { continue; };

void solve_puzzle(Puzzle* puzzle);

#endif // SRC_SOLVERS_LOGICAL_LOGICAL_SOLVE_H
