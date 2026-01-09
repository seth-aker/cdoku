#include <stdio.h>
#include "logger.h"
#include "puzzle.h"
#include "sudoku_solver.h"
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
void printSummary(StepSummary* summary) {
  printf("\n");
  printf("================ Summary =================\n");
  printf("TOTAL STEPS: %d\n", summary->totalSteps);
  printf("SOLVED: %s\n", summary->solved ? "TRUE" : "FALSE");
  printf("Strategies Used:\n");
  printf("Full Houses: %d\n", summary->fullHouseCount);
  printf("Naked Singles: %d\n", summary->nakedSingleCount);
  printf("Hidden Singles: %d\n", summary->hiddenSingleCount);
  printf("Naked Pairs: %d\n", summary->nakedPairsCount);
  printf("Hidden Pairs: %d\n", summary->nakedSingleCount);
  printf("Naked Triples: %d\n", summary->nakedTriplesCount);
  printf("Hidden Triples: %d\n", summary->hiddenTriplesCount);
  printf("Naked Quads: %d\n", summary->nakedQuadsCount);
  printf("Hidden Quads: %d\n", summary->hiddenQuadsCount);
  printf("X-Wings: %d\n", summary->xWingCount);
  printf("Finned X-Wings: %d\n", summary->finnedXWingCount);
  printf("Swordfish: %d\n", summary->swordfishCount);
  printf("Finned Swordfish: %d\n", summary->finnedSwordfishCount);
  printf("Jellyfish: %d\n", summary->jellyfishCount);
  printf("Finned Jellyfish: %d\n", summary->finnedJellyfishCount);
  printf("Guesses: %d\n", summary->guessCount);
  printf("=========================================\n");
}
void printStep(Step step, int stepNumber) {
  printf("\n");
  if(stepNumber != -1) {
    printf("Step: %d\n", stepNumber);
  }
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
  getStrategyName(step.strategyUsed, strategyName);
  printf("STRATEGY USED: %s\n", strategyName);
  printf("\n=========================================\n");
}