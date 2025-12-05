#include "logger.h"

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
void printStep(Step step, int stepNumber) {
  printf("\n");
  printf("Step: %d\n", stepNumber);
  printf("(ROW, COLUMN): (%d, %d)\n", step.rowIndex, step.colIndex);
  if(step.value) {
    printf("VALUE: %d\n", step.value);
  }
  if(step.candidatesRemoved) {
    int candidates[9];
    printf("Candidates Removed: ");
    int count = getCandidatesInCell(step.candidatesRemoved, candidates);
    for(int i = 0; i < count; ++i) {
      printf("%d, ", candidates[i]);
    }
    printf("\n");
  }
  char strategyName[27];
  getStrategyName(step.strategyUsed, &strategyName);
  printf("STRATEGY USED: %s", strategyName);
}