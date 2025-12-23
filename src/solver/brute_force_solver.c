#include "step.h"
#include "puzzle.h"
StepNode* makeGuess(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  int cellIndex = findEmptyCell(puzzle);
  if(cellIndex == -1 && isPuzzleSolved(puzzle)) {
    return current;
  }

}

int findEmptyCell(Puzzle* puzzle) {
  for(int i = 0; i < PUZZLE_WIDTH * PUZZLE_WIDTH; ++i) {
    if(puzzle->cells[i] == 0) {
      return i;
    }
  }
  return -1;
}