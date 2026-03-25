#ifndef SRC_PUZZLE_PUZZLE_H
#define SRC_PUZZLE_PUZZLE_H

#include "stdint.h"
#include "stdbool.h"
#include "step.h"

#define BLOCK_WIDTH 3
#define PUZZLE_WIDTH 9
#define TOTAL_CELLS 81

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

typedef struct House {
  uint16_t candidates[PUZZLE_WIDTH];
  uint8_t cells[PUZZLE_WIDTH];
} House;

void get_row(uint8_t idx, Puzzle* puzzle, House* row);
void get_col(uint8_t idx, Puzzle* puzzle, House* col);
void get_block(uint8_t idx, Puzzle* puzzle, House* block);

int find_empty_cell(uint8_t cells[]);
int pasre_puzzle_str(char puzzle_str[], Puzzle* puzzle);
bool is_valid_num_in_cell(uint8_t num, int idx, uint8_t cells[]);

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

static inline int get_cell_pos_in_block(uint8_t row_idx, uint8_t col_idx) {
  return ROW_NORM[row_idx] + COL_NORM[col_idx];
}

extern const uint8_t ROW_NORM[PUZZLE_WIDTH];
extern const uint8_t COL_NORM[PUZZLE_WIDTH];
extern const uint8_t IDX_TO_ROW[TOTAL_CELLS];
extern const uint8_t IDX_TO_COL[TOTAL_CELLS];
extern const uint8_t IDX_TO_BLOCK[TOTAL_CELLS];
extern const uint8_t CELL_PEERS_LOOKUP[TOTAL_CELLS][20];

#endif //SRC_PUZZLE_PUZZLE_H
