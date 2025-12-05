#include <stdio.h>
#include "solver/sudoku_solver.h"
#include "solver/step.h"
#include "solver/puzzle.h"

int main() {
  printf("Starting solver\n");
  StepNode* stepList = initStepList();
  Puzzle puzzle = {
    .candidates = {0},
    .cells = {
      9, 0, 4, 0, 5, 3, 0, 0, 0,
      0, 0, 0, 0, 6, 0, 0, 0, 0,
      6, 0, 0, 7, 2, 0, 5, 0, 0,
      2, 9, 0, 0, 8, 0, 7, 0, 0,
      0, 0, 3, 0, 0, 0, 0, 5, 0,
      0, 0, 0, 0, 1, 0, 0, 4, 8,
      0, 0, 6, 0, 0, 2, 0, 1, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 7,
      0, 0, 0, 0, 9, 0, 0, 0, 6
    },
    .id = 1
  };
  bool isSolved = solvePuzzle(&puzzle, stepList);
  if(isSolved) {
    printf("Puzzle solved!\n");
    printCells(puzzle.cells);
  } else {
    printf("Puzzle failed to solve\n");
    printCells(puzzle.cells);
  }
  freeStepList(stepList);
  return 0;
}