#ifndef SRC_GENERATOR_PUZZLE_GENERATOR_H
#define SRC_GENERATOR_PUZZLE_GENERATOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "puzzle.h"
void generate_puzzle(Puzzle* puzzle, DiffRating target_difficulty);
bool fill_puzzle_randomly(uint8_t cells[]);
bool remove_random_val(Puzzle* puzzle);
void reset_puzzle(Puzzle* puzzle);
int define_min_cells_removed(DiffRating target_difficulty);

inline static void clone_puzzle(Puzzle* dest, const Puzzle* src) {
  memcpy(dest, src, sizeof(Puzzle));
}
#endif // SRC_GENERATOR_PUZZLE_GENERATOR_H
