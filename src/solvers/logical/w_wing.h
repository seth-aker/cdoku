#ifndef SRC_SOLVERS_LOGICAL_W_WING_H
#define SRC_SOLVERS_LOGICAL_W_WING_H
#include <stdbool.h>
#include <stdint.h>
#include "puzzle.h"
#include "utils.h"
TechniqueResult find_w_wings(Puzzle* puzzle);
bool check_w_wing_link(Puzzle* puzzle, Array_UInt8* bin, int link_val, uint16_t eliminate_mask);
#endif // SRC_SOLVERS_LOGICAL_W_WING_H
