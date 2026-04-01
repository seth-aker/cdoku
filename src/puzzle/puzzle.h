#ifndef SRC_PUZZLE_PUZZLE_H
#define SRC_PUZZLE_PUZZLE_H

#include "stdint.h"
#include "stdbool.h"
#include "step.h"
#include "puzzle_globals.h"

typedef enum DiffRating : uint8_t {
  BEGINNER,
  EASY,
  MEDIUM,
  HARD,
  IMPOSSIBLE
} DiffRating;

typedef struct PuzzleDiff {
  uint32_t score;
  DiffRating rating;
  Technique hardest_step;
} PuzzleDiff;

typedef struct Puzzle {
  Step solution[729];
  PuzzleDiff difficulty;
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
void set_diff_rating(Puzzle* puzzle);
int stringify_puzzle(char dest[], int dest_size, const Puzzle* puzzle);
int parse_puzzle_str(const char puzzle_str[], Puzzle* puzzle);
bool is_valid_num_in_cell(uint8_t num, int idx, uint8_t cells[]);
bool is_puzzle_solved(uint8_t cells[]);

static inline bool has_candidate(uint16_t mask, uint16_t num) {
  return (mask >> (num - 1)) & 1;
}
static inline void add_candidate(uint16_t* mask, uint16_t num) {
  *mask |= (1 << (num - 1));
}

static inline void log_step(Puzzle* puzzle, Step step) {
  puzzle->solution[puzzle->step_count++] = step;
  puzzle->difficulty.score += (uint32_t)step.technique;
  if(puzzle->difficulty.hardest_step < step.technique) {
    puzzle->difficulty.hardest_step = step.technique;
  }
}

#endif //SRC_PUZZLE_PUZZLE_H
