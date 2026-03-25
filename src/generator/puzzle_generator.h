#ifndef SRC_GENERATOR_PUZZLE_GENERATOR_H
#define SRC_GENERATOR_PUZZLE_GENERATOR_H

#include <stdbool.h>
#include <stdint.h>
void generate_puzzle(DiffRating targetDiff, uint8_t cells[], int* diffScore);
bool fill_puzzle_randomly(uint8_t cells[]);
bool remove_random_val(uint8_t cells[]);
#endif // SRC_GENERATOR_PUZZLE_GENERATOR_H
