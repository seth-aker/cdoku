#include "logical_solve.h"
#include "singles.h"

void solve_puzzle(Puzzle* puzzle) {
  TechiqueResult puzzle_progress = NO_PROGRESS;
  while (!is_puzzle_solved(puzzle->cells)) {
    puzzle_progress = find_naked_single(puzzle);
    CHECK_PROGRESS(puzzle_progress);

    puzzle_progress = find_hidden_single(puzzle);
    CHECK_PROGRESS(puzzle_progress)


  }


}
