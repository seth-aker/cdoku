#ifndef SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#define SRC_SOLVERS_LOGICAL_SIMPLE_COLORS
#include "stdint.h"
#include "puzzle.h"
#include "step.h"
#include "chain_utils.h"

TechniqueResult find_simple_color(Puzzle* puzzle);

int paint_colors(ChainSearchContext* context, uint8_t start_cell);

void apply_val_to_chain_color(Puzzle* puzzle, ChainSearchContext* context, uint8_t color, uint8_t value);

#endif
