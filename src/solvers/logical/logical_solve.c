#include <stdbool.h>
#include "logical_solve.h"
#include "singles.h"
#include "subsets.h"
#include "locked_candidates.h"
#include "x_wing.c"
#include "skyscraper.h"
#include "xy_wing.h"

bool solve_puzzle(Puzzle* puzzle) {
  TechniqueResult puzzle_progress = NO_PROGRESS;
  while(!is_puzzle_solved(puzzle->cells) && puzzle_progress != INVALID_STATE) {
    // Beginner solutions
    puzzle_progress = find_naked_single(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = find_hidden_single(puzzle);
    CHECK_PROGRESS(puzzle_progress);

    // EASY solutions
    puzzle_progress = search_naked_pairs(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = search_naked_triples(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = find_locked_candidate_pointing(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = find_locked_candidate_claiming(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = search_hidden_pairs(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = search_hidden_triples(puzzle);
    CHECK_PROGRESS(puzzle_progress);

    // MEDIUM solutions
    puzzle_progress = find_x_wing(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = find_skyscrapers(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = find_xy_wings(puzzle);
    CHECK_PROGRESS(puzzle_progress);
    puzzle_progress = INVALID_STATE;
  }
  if(puzzle_progress == INVALID_STATE) {
    return false;
  }
  set_diff_rating(puzzle);
  return true;
}

