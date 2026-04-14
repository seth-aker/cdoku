#ifndef SRC_SOLVERS_LOGICAL_LOGICAL_SOLVE_H
#define SRC_SOLVERS_LOGICAL_LOGICAL_SOLVE_H
#include <stdbool.h>
#include "puzzle.h"
#include "step.h"
#define CHECK_PROGRESS(progress) if(progress == PROGRESS_MADE) { continue; };

bool solve_puzzle(Puzzle* puzzle);

#endif // SRC_SOLVERS_LOGICAL_LOGICAL_SOLVE_H
