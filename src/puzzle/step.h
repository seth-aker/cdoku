#ifndef SRC_PUZZLE_STEP_H
#define SRC_PUZZLE_STEP_H

#include <stdint.h>

struct Puzzle;
typedef enum : uint16_t {
  FULL_HOUSE = 10,
  NAKED_SINGLE = 20,
  HIDDEN_SINGLE = 40,
  LOCKED_POINTING = 150,
  LOCKED_CLAIMING = 200,
  NAKED_PAIR = 300,
  HIDDEN_PAIR = 400,
  NAKED_TRIPLE = 500,
  HIDDEN_TRIPLE = 700,
  NAKED_QUAD = 900,
  HIDDEN_QUAD = 1200,
  X_WING = 1500,
  SKYSCRAPER = 1800,
  XY_WING = 2000,
  SIMPLE_COLORS = 2200,
  FINNED_X_WING = 2500,
  XYZ_WING = 4000,
  SWORDFISH = 5000,
  FINNED_SWORDFISH = 6000,
  JELLYFISH = 8000,
  FORCING_CHAIN = 15000,
  GUESS = 25000
} Technique;

typedef enum TechniqueResult {
  INVALID_STATE = -1,
  NO_PROGRESS = 0,
  PROGRESS_MADE = 1
} TechniqueResult;

typedef struct Step {
  uint16_t eliminated_mask;
  Technique technique;
  uint8_t target_cell;
  uint8_t placed_val;
} Step;

void apply_step(struct Puzzle* puzzle, Step step);
const char* technique_to_string(Technique techinque);
#endif // SRC_PUZZLE_STEP_H
