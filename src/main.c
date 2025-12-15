#include <stdio.h>
#include "step.h"
#include "puzzle.h"
#include "logger.h"
#include "sudoku_solver.h"
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
  StepSummary summary = { 0 };
  if(isSolved) {
    printf("Puzzle solved!\n");
    printCells(puzzle.cells);
    summary.solved = true;
  } else {
    printf("Puzzle failed to solve\n");
    printCells(puzzle.cells);
    summary.solved = false;
  }
  getStepSummary(&summary, stepList);
  printSummary(&summary);
  freeStepList(stepList);
  return 0;
}