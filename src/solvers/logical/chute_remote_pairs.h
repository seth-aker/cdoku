#ifndef SRC_SOLVERS_LOGICAL_CHUTE_REMOTE_PAIRS_H
#define SRC_SOLVERS_LOGICAL_CHUTE_REMOTE_PAIRS_H
#include "puzzle.h"
#include "stdint.h"
#include "stdbool.h"

TechniqueResult find_chute_remote_pairs(Puzzle* puzzle);
bool eliminate_from_intersections(Puzzle* puzzle, uint16_t eliminate_mask, uint8_t idx_one, uint8_t idx_two);
#endif // SRC_SOLVERS_LOGICAL_CHUTE_REMOTE_PAIRS_H
