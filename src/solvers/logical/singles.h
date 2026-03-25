#ifndef SRC_SOLVERS_LOGICAL_SINGLES_H
#define SRC_SOLVERS_LOGICAL_SINGLES_H

#include <stdbool.h>
#include "puzzle.h"

bool find_naked_single(Puzzle* puzzle);
bool find_hidden_single(Puzzle* puzzle);
#endif //  SRC_SOLVERS_LOGICAL_SINGLES_H
