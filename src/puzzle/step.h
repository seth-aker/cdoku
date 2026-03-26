#ifndef SRC_PUZZLE_STEP_H
#define SRC_PUZZLE_STEP_H

#include <stdint.h>

struct Puzzle;
typedef enum : uint8_t {
  FULL_HOUSE,
  NAKED_SINGLE,
  HIDDEN_SINGLE,
  NAKED_PAIR,
  HIDDEN_PAIR,
  NAKED_TRIPLE,
  HIDDEN_TRIPLE,
  NAKED_QUAD,
  HIDDEN_QUAD,
  LOCKED_POINTING,
  LOCKED_CLAIMING,
  X_WING,
  FINNED_X_WING,
  SKYSCRAPER,
  XY_WING,
  SIMPLE_COLORS,
  SWORDFISH,
  FINNED_SWORDFISH,
  JELLYFISH,
  XYZ_WING,
  FORCING_CHAIN,
  GUESS
} Technique;

typedef enum TechiqueResult {
  INVALID_STATE = -1,
  NONE_FOUND = 0,
  PROGRESS_MADE = 1
} TechiqueResult;

typedef struct Step {
  uint16_t eliminated_mask;
  Technique technique;
  uint8_t target_cell;
  uint8_t placed_val;
} Step;

void apply_step(struct Puzzle* puzzle, Step step);

#endif // SRC_PUZZLE_STEP_H
