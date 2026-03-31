#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include "log.h"
#include "puzzle.h"
#include "puzzle_globals.h"

int find_empty_cell(uint8_t cells[]) {
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(cells[i] == 0) {
      return i;
    }
  }
  return -1;
}
void set_diff_rating(Puzzle* puzzle) {
  Technique hardest_step = puzzle->difficulty.hardest_step;
  if(hardest_step <= HIDDEN_SINGLE) {
    puzzle->difficulty.rating = BEGINNER;
  } else if(hardest_step <= HIDDEN_TRIPLE) {
    puzzle->difficulty.rating = EASY;
  } else if(hardest_step <= SIMPLE_COLORS) {
    puzzle->difficulty.rating = MEDIUM;
  } else if(hardest_step <= JELLYFISH) {
    puzzle->difficulty.rating = HARD;
  } else {
    puzzle->difficulty.rating = IMPOSSIBLE;
  }
}
int pasre_puzzle_str(char puzzle_str[], Puzzle* puzzle) {
  char* endptr;
  errno = 0;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    char* str = &puzzle_str[i];
    long num = strtol(str, &endptr, 10);
    if(endptr == str || *endptr != '\0') {
      log_error("Invalid Puzzle String! %s", str);
      return 1;
    } else if(errno == ERANGE) {
      log_error("Number out of range! %s", str);
      return 1;
    } else if(num < 0 || num > 9) {
      log_error("Number invalid: %ld", num);
      return 1;
    }
    puzzle->cells[i] = (int)num;
  }
  return 0;
}

bool is_valid_num_in_cell(uint8_t num, int idx, uint8_t cells[]) {
  if(cells[idx] != 0) {
    return false;
  }
  const uint8_t* peers = CELL_PEERS_LOOKUP[idx];
  for(int i = 0; i < 20; ++i) {
    int peer_idx = peers[i];
    if(cells[peer_idx] == num) {
      return false;
    }
  }
  return true;
}
void get_row(uint8_t idx, Puzzle* puzzle, House* row) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    uint8_t offset = idx * 9;
    row->candidates[i] = puzzle->candidates[offset + i];
    row->cells[i] = puzzle->cells[offset + i];
    row->idx_lookup[i] = ROW_TO_IDXS[idx][i];
  }
}
void get_col(uint8_t idx, Puzzle* puzzle, House* col) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    uint8_t offset = i * 9;
    col->candidates[i] = puzzle->candidates[offset + idx];
    col->cells[i] = puzzle->cells[offset + idx];
    col->idx_lookup[i] = COL_TO_IDXS[idx][i];
  }
}
void get_block(uint8_t idx, Puzzle* puzzle, House* block) {
  const uint8_t* block_idxs = BLOCK_TO_IDXS[idx];
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    block->candidates[i] = puzzle->candidates[block_idxs[i]];
    block->cells[i] = puzzle->cells[block_idxs[i]];
    block->idx_lookup[i] = BLOCK_TO_IDXS[idx][i];
  }
}
bool is_puzzle_solved(uint8_t cells[]) {
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(cells[i] == 0 || !is_valid_num_in_cell(cells[i], i, cells)) {
      return false;
    }
  }
  return true;
}
