#include "locked_candidates.h"

TechiqueResult find_locked_candidate_pointing(Puzzle* puzzle) {
  House block;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    get_block(i, puzzle, &block);
    uint16_t row1 = block.candidates[0] | block.candidates[1] | block.candidates[2];
    uint16_t row2 = block.candidates[3] | block.candidates[4] | block.candidates[5];
    uint16_t row3 = block.candidates[6] | block.candidates[7] | block.candidates[8];

    uint16_t unique_row1 = row1 & ~(row2 | row3);
    uint16_t unique_row2 = row2 & ~(row1 | row3);
    uint16_t unique_row3 = row3 & ~(row1 | row2);

    if(unique_row1) {

    }


  }
}

bool removed_pointing_row(Puzzle* puzzle, House* block, uint8_t row_idx) {
  House row;
  get_row(row_idx, puzzle, &row);
}
