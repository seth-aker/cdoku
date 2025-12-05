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
void printStep(Step step) {
  
}