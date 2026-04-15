#include "guess.h"
#include "step.h"
#include "puzzle.h"
#include "utils.h"
#include "stdlib.h"
#include "string.h"
#include "logical_solve.h"
TechniqueResult make_guess(Puzzle* puzzle) {
  int cell_idxs[81];
  int count = 0;
  int cand_count = 2;
  do {
    count = get_cells_with_candidates_count(puzzle, cell_idxs, cand_count);
    cand_count++;
  } while(count < 1 && cand_count <= 9);

  if(count < 1) {
    return NO_PROGRESS;
  }

  int rand_idx = rand() % count;
  uint16_t cell_cands = puzzle->candidates[cell_idxs[rand_idx]];

  int guess_cand = __builtin_ctz(cell_cands) + 1;

  Puzzle temp;
  memcpy(&temp, puzzle, sizeof(Puzzle));

  Step guess_step = {
    .eliminated_mask = cell_cands,
    .placed_val = guess_cand,
    .target_cell = cell_idxs[rand_idx],
    .technique = GUESS
  };
  apply_step(&temp, guess_step);

  if(solve_puzzle(&temp)) {
    memcpy(puzzle, &temp, sizeof(Puzzle));
    return PROGRESS_MADE;
  }

  // if solve_puzzle returns false, then the guess candidate is certainly false.
  Step nishio_step = {
    .eliminated_mask = 1 << (guess_cand - 1),
    .placed_val = 0,
    .target_cell = cell_idxs[rand_idx],
    .technique = GUESS
  };
  apply_step(puzzle, nishio_step);
  return PROGRESS_MADE;
}
