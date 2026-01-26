#include <stdio.h>
#include <stdarg.h>
#include "logger.h"
#include "puzzle.h"
#include "sudoku_solver.h"
#include "log.h"

void setupLogger() {
  char nameBuf[100];
  time_t now = time(NULL);
  struct tm *timenow = localtime(&now);

  strftime(nameBuf, sizeof(nameBuf), "logs/%Y-%m-%d_%H:%M:%S.log", timenow);
  FILE* log_file = fopen(nameBuf, "a");
  if(log_file == NULL) {
    perror(("Error opening log file"));
    exit(EXIT_FAILURE);
  }
  log_add_fp(log_file, LOG_LEVEL);

}

void printRow(int rowNum, int* cells, char* dest) {
  int strLenth = strlen(dest);
  strcat(dest, "|");
  strLenth++;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int rowOffset = rowNum * PUZZLE_WIDTH;
    snprintf(&dest[strLenth], 1000 - strLenth, "| %d ", cells[i + rowOffset]);
    strLenth = strlen(dest);
    if(i % BLOCK_WIDTH == 2) {
      strcat(dest, "|");
      strLenth++;
    }
  }
  strcat(dest,"|\n");
}
void printCells(int* cells) {
  char puzzleStr[1000];
  strcat(puzzleStr,"=========================================\n");
  
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    printRow(i, cells, puzzleStr);
    if(i % BLOCK_WIDTH == 2) {
      strcat(puzzleStr,"=========================================\n");
    }
  }
  log_info("\n%s", puzzleStr);
}
void printSummary(StepSummary* summary) {
  log_info("\n");
  log_info("================ Summary =================\n");
  log_info("TOTAL STEPS: %d\n", summary->totalSteps);
  log_info("SOLVED: %s\n", summary->solved ? "TRUE" : "FALSE");
  log_info("Strategies Used:\n");
  log_info("Full Houses: %d\n", summary->fullHouseCount);
  log_info("Naked Singles: %d\n", summary->nakedSingleCount);
  log_info("Hidden Singles: %d\n", summary->hiddenSingleCount);
  log_info("Naked Pairs: %d\n", summary->nakedPairsCount);
  log_info("Hidden Pairs: %d\n", summary->nakedSingleCount);
  log_info("Naked Triples: %d\n", summary->nakedTriplesCount);
  log_info("Hidden Triples: %d\n", summary->hiddenTriplesCount);
  log_info("Naked Quads: %d\n", summary->nakedQuadsCount);
  log_info("Hidden Quads: %d\n", summary->hiddenQuadsCount);
  log_info("X-Wings: %d\n", summary->xWingCount);
  log_info("Finned X-Wings: %d\n", summary->finnedXWingCount);
  log_info("Swordfish: %d\n", summary->swordfishCount);
  log_info("Finned Swordfish: %d\n", summary->finnedSwordfishCount);
  log_info("Jellyfish: %d\n", summary->jellyfishCount);
  log_info("Finned Jellyfish: %d\n", summary->finnedJellyfishCount);
  log_info("Guesses: %d\n", summary->guessCount);
  log_info("=========================================\n");
}
void printStep(Step step, int stepNumber) {
  log_info("\n");
  if(stepNumber != -1) {
    log_info("Step: %d\n", stepNumber);
  }
  log_info("(ROW, COLUMN): (%d, %d)\n", step.rowIndex, step.colIndex);
  if(step.value) {
    log_info("VALUE: %d\n", step.value);
  }
  if(step.candidatesRemoved) {
    int candidates[9];
    log_info("Candidates Removed: ");
    int count = getCandidatesInCell(step.candidatesRemoved, candidates);
    for(int i = 0; i < count; ++i) {
      log_info("%d, ", candidates[i]);
    }
    log_info("\n");
  }
  char strategyName[27];
  getStrategyName(step.strategyUsed, strategyName);
  log_info("STRATEGY USED: %s\n", strategyName);
  log_info("\n=========================================\n");
}