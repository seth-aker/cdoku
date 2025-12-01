#include <stdio.h>
#include "solver/sudoku_solver.h"
#include "solver/strategies.h"
#include "solver/puzzle.h"
void printRow(int rowNum, int* cells) {
  printf("|");
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int rowOffset = rowNum * PUZZLE_WIDTH;
    printf("| %d ", cells[i + rowOffset]);
    if(i % BLOCK_WIDTH == 2) {
      printf("|");
    }
  }
  printf("|\n");
} 
void printCells(int* cells) {
  printf("=========================================\n");
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    printRow(i, cells);
    if(i % BLOCK_WIDTH == 2) {
      printf("=========================================\n");
    }
  }
}
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