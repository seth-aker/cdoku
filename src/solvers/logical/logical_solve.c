#include "logical_solve.h"
#include "singles.h"
#include "subsets.h"
#include "locked_candidates.h"

void solve_puzzle(Puzzle* puzzle) {
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
    puzzle_progress = INVALID_STATE;
  }
  set_diff_rating(puzzle);

}

