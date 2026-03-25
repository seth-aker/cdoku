#include <step.h>
#include <puzzle.h>

void apply_step(Puzzle* puzzle, Step step) {
  const uint8_t* peers = CELL_PEERS_LOOKUP[step.target_cell];
  if (step.placed_val != 0) {
    puzzle->cells[step.target_cell] = step.placed_val;
    puzzle->candidates[step.target_cell] = 0;
  }
  else {
    puzzle->candidates[step.target_cell] &= ~step.eliminated_mask;
  }

  log_step(puzzle, step);
  for (int i = 0; i < 20; ++i) {
    uint8_t peer_idx = peers[i];
    if (puzzle->cells[peer_idx] != 0) {
      continue;
    }
    uint16_t vals_eliminated = (puzzle->candidates[peer_idx] & step.eliminated_mask);
    if (vals_eliminated != 0) {
      puzzle->candidates[peer_idx] &= ~step.eliminated_mask;
      Step anotherStep = step;
      anotherStep.target_cell = peer_idx;
      anotherStep.eliminated_mask = vals_eliminated;
      log_step(puzzle, anotherStep);
    }
  }
}


