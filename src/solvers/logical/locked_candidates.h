#ifndef SRC_SOLVERS_LOGICAL_LOCKED_CANDIDATES_H
#define SRC_SOLVERS_LOGICAL_LOCKED_CANDIDATES_H
#include "puzzle.h"
#include "step.h"

TechniqueResult find_locked_candidate_pointing(Puzzle* puzzle);
bool remove_pointing_row(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t block_row_idx);
bool remove_pointing_col(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t block_col_idx);

TechniqueResult find_locked_candidate_claiming(Puzzle* puzzle);
bool remove_claiming_row(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t skip_row_start);
bool remove_claiming_col(Puzzle* puzzle, uint16_t mask_to_remove, uint8_t block_idx, uint8_t skip_col_idx);

#endif // SRC_SOLVERS_LOGICAL_LOCKED_CANDIDATES_H
