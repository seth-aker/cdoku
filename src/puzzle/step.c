#include "step.h"
#include "puzzle.h"
#include "puzzle_globals.h"
#include "log.h"
#include "utils.h"

void apply_step(Puzzle* puzzle, Step step) {
  const uint8_t* peers = CELL_PEERS_LOOKUP[step.target_cell];
  log_step(puzzle, step);
  update_score(puzzle, step.technique);
  if(step.placed_val != 0) {
    log_debug("Value %d placed at cell index %d using %s", step.placed_val, step.target_cell, technique_to_string(step.technique));
    puzzle->cells[step.target_cell] = step.placed_val;
    puzzle->candidates[step.target_cell] = 0;
    uint16_t placed_mask = 1 << (step.placed_val - 1);
    for(int i = 0; i < 20; ++i) {
      uint8_t peer_idx = peers[i];
      if(puzzle->cells[peer_idx] != 0) {
        continue;
      }
      uint16_t vals_eliminated = (puzzle->candidates[peer_idx] & placed_mask);
      if(vals_eliminated) {
        puzzle->candidates[peer_idx] &= ~placed_mask;
        Step propagated_step = {
          .target_cell = peer_idx,
          .eliminated_mask = vals_eliminated,
          .placed_val = 0,
          .technique = step.technique
        };
        log_step(puzzle, propagated_step);
      }
    }
  } else {
    puzzle->candidates[step.target_cell] &= ~step.eliminated_mask;
  }
}

const char* technique_to_string(Technique techinque) {
  switch(techinque) {

#define EP(x) case x: return #x;

    EP(FULL_HOUSE)
      EP(NAKED_SINGLE)
      EP(HIDDEN_SINGLE)
      EP(LOCKED_POINTING)
      EP(LOCKED_CLAIMING)
      EP(NAKED_PAIR)
      EP(HIDDEN_PAIR)
      EP(NAKED_TRIPLE)
      EP(HIDDEN_TRIPLE)
      EP(NAKED_QUAD)
      EP(HIDDEN_QUAD)
      EP(X_WING)
      EP(SKYSCRAPER)
      EP(XY_WING)
      EP(SIMPLE_COLORS)
      EP(FINNED_X_WING)
      EP(XYZ_WING)
      EP(SWORDFISH)
      EP(FINNED_SWORDFISH)
      EP(JELLYFISH)
      EP(FORCING_CHAIN)
      EP(GUESS)

#undef EP
  default: return "UNKNOWN_TECHNIQUE";
  }
}


