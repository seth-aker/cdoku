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

int stringify_puzzle(char dest[], int dest_size, const Puzzle* puzzle, const uint8_t solved_cells[]) {
  if(dest_size < 180) {// dest need to be: 81 cells + ':' + 81 solved cells + ':' + max score (10) + ':' + max rating(2) + '\0';
    dest[0] = '\0';
    return -1;
  }
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    dest[i] = puzzle->cells[i] + '0'; // int to ascii conversion
    dest[i + 82] = solved_cells[i] + '0';
  }
  dest[81] = ':'; // place colon between puzzle start and puzzle solution

  return snprintf(dest + ((TOTAL_CELLS * 2) + 1), dest_size - ((TOTAL_CELLS * 2) + 1), ":%u:%d",
    puzzle->difficulty.score,
    puzzle->difficulty.rating);

}
int parse_puzzle_str(const char puzzle_str[], Puzzle* puzzle) {
  char* endptr;
  errno = 0;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    const char* str = &puzzle_str[i];
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
  const uint8_t* peers = CELL_PEERS_LOOKUP[idx];
  for(int i = 0; i < 20; ++i) {
    uint8_t peer_idx = peers[i];
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
    block->idx_lookup[i] = block_idxs[i];
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
void fill_puzzle_candidates(Puzzle* puzzle) {
  for(int idx = 0; idx < TOTAL_CELLS; ++idx) {
    if(puzzle->cells[idx] != 0) {
      puzzle->candidates[idx] = 0;
      continue;
    }
    uint16_t new_mask = 0;
    for(int i = 1; i < 10; ++i) {
      if(is_valid_num_in_cell(i, idx, puzzle->cells)) {
        new_mask |= (uint16_t)(1 << (i - 1));
      }
    }
    puzzle->candidates[idx] = new_mask;
  }
}

int get_candidate_positions(const Puzzle* puzzle, const uint8_t house_idxs[], SearchParams params, int out_pos[]) {
  int candidate_count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    uint16_t has_candidate = puzzle->candidates[house_idxs[i]] & params.mask;
    if(has_candidate) {
      candidate_count++;
      if(candidate_count > params.max_count) {
        return candidate_count;
      }
      out_pos[candidate_count - 1] = i;
    }
  }
  return candidate_count;
}

void print_puzzle_state(Puzzle* puzzle) {
  char puzzle_str_buff[200];
  uint8_t dummy[81] = { 0 };
  int written = stringify_puzzle(puzzle_str_buff, 200, puzzle, dummy);
  log_debug("Puzzle State: %s", puzzle_str_buff);
  log_debug("Solution Trace:");
  for(int i = 0; i < puzzle->step_count; ++i) {
    Step step = puzzle->solution[i];
    int row = step.target_cell / PUZZLE_WIDTH;
    int col = step.target_cell % PUZZLE_WIDTH;
    if(step.placed_val) {
      log_debug("STEP %d: r%dc%d: Placed %d", i + 1, row, col, step.placed_val);
    } else {
      char candidates_removed_buff[20];
      int removed_buff_len = 0;
      uint16_t candidates = step.eliminated_mask;
      while(candidates != 0) {
        int candidate = __builtin_ctz(candidates) + 1;
        candidates_removed_buff[removed_buff_len++] = candidate + '0';
        candidates &= (candidates - 1);
        if(candidates != 0) {
          candidates_removed_buff[removed_buff_len++] = ',';
        }
      }
      candidates_removed_buff[removed_buff_len] = '\0';
      log_debug("STEP %d: r%dc%d: Removed candiates %s.\n    Technique: %s", i + 1, row, col, candidates_removed_buff, technique_to_string(step.technique));
    }
  }
}

int get_cells_with_candidates_count(const Puzzle* puzzle, int dest_idxs[], int num_of_cands) {
  int count = 0;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(__builtin_popcount(puzzle->candidates[i]) == num_of_cands) {
      dest_idxs[count++] = i;
    }
  }
  return count;
}
