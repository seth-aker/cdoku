#ifndef SRC_GENERATOR_PUZZLE_GENERATOR_H
#define SRC_GENERATOR_PUZZLE_GENERATOR_H

#include <stdbool.h>
#include <stdint.h>
void generatePuzzle(DiffRating targetDiff, uint8_t cells[], int* diffScore);
bool fillPuzzleRandomly(uint8_t cells[]);
bool removeRandomVal(uint8_t cells[]);
#endif // SRC_GENERATOR_PUZZLE_GENERATOR_H
