#ifndef SRC_PUZZLE_PUZZLE_H
#define SRC_PUZZLE_PUZZLE_H

#include "stdint.h"
#include "stdbool.h"
#include "step.h"
#include "globals.h"

typedef enum DiffRating {
  BEGINNER,
  EASY,
  MEDIUM,
  HARD,
  IMPOSSIBLE
} DiffRating;

typedef struct Puzzle {
  Step solution[729];
  uint16_t candidates[TOTAL_CELLS];
  uint8_t cells[TOTAL_CELLS];
  uint16_t step_count;
} Puzzle;

typedef enum : uint8_t {
  ROW,
  COL,
  BLOCK
} HouseType;
typedef struct House {
  uint16_t candidates[PUZZLE_WIDTH];
  uint8_t cells[PUZZLE_WIDTH];
  uint8_t idx_lookup[PUZZLE_WIDTH];
} House;

void get_row(uint8_t idx, Puzzle* puzzle, House* row);
void get_col(uint8_t idx, Puzzle* puzzle, House* col);
void get_block(uint8_t idx, Puzzle* puzzle, House* block);

int find_empty_cell(uint8_t cells[]);
int pasre_puzzle_str(char puzzle_str[], Puzzle* puzzle);
bool is_valid_num_in_cell(uint8_t num, int idx, uint8_t cells[]);
bool is_puzzle_solved(uint8_t cells[]);

static inline bool has_candidate(uint16_t mask, uint8_t num) {
  return (mask >> (num - 1)) & 1;
}
static inline void add_candidate(uint16_t* mask, uint8_t num) {
  *mask |= (1 << (num - 1));
}
static inline bool remove_candidate(uint16_t* mask, uint8_t num) {
  bool removed = has_candidate(*mask, num);
  *mask &= ~(1 << (num - 1));
  return removed;
}

static inline void log_step(Puzzle* puzzle, Step step) {
  puzzle->solution[puzzle->step_count] = step;
  puzzle->step_count++;
}

#endif //SRC_PUZZLE_PUZZLE_H
