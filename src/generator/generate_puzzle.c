#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "generate_puzzle.h"
#include "sudoku_solver.h"
#include "logger.h"
#include "utils.h"
#include "log.h"


int main(int argc, char* argv[]) {

  srand((unsigned)time(NULL));

  setupLogger();
  log_info("Running generator...\n");
  if(argc != 3) {
    log_info("Incorrect number of arguments: Expected 3, recieved %d\n", argc);
    return 1;
  }
  log_info("Arg count: %d\n", argc);
  log_info("Parsing Arg[1]: %s\n", argv[1]);
  int puzzleCount = parsePuzzleCount(argv[1]);
  // int puzzleCount = 1;
  if(puzzleCount == -1) {
    return 1;
  }

  log_info("Parsing Arg[2]: %s\n", argv[2]);
  int difficulty = parseDifficultyArg(argv[2]);
  // int difficulty = EASY;
  if(difficulty == -1) {
    return 1;
  }
  log_info("Found difficulty: %d\n", difficulty);

  int cells[81] = {0};
  int difficultyScore = 0;
  char puzzleString[90];
  
  log_info("Generating %d Puzzles...\n", puzzleCount);
  for(int i = 0; i < puzzleCount; i++) {
    log_info("Generating puzzle number: %d\n", i + 1);
    generatePuzzle(difficulty, cells, &difficultyScore);
    log_info("Puzzle %d Generation finished.\n", i);
    puzzleString[0] = '\0'; // ensure string is empty.
    formatCellsToString(cells, puzzleString);

    // this prints to node in the following format
    // [cellValues],[difficultyScore];
    // e.g. 000000000...x81 numbers...000,12345;
    printf("%s,%d\n", puzzleString, difficultyScore);
    log_info("Puzzle #%d\n", i + 1);
    printCells(cells);
  }
}

void generatePuzzle(DifficultyRating targetDifficulty, int* cells, int* difficultyScore) {
  int cellsRemovedCount = 0;
  int MIN_CELLS_REMOVED = (rand() % 15) + 25; // generate a random number of min cells removed between 25 and 39 inclusive 
  int minDifficultyScore = 0;
  int maxDifficultyScore = 0;
  setMinMaxDifficulty(&minDifficultyScore, &maxDifficultyScore, targetDifficulty);
  bool puzzleAtDifficulty = false;
  fillPuzzleRandomly(cells);
  while(!puzzleAtDifficulty) {
    // If removeRandomValue fails to result in a valid puzzle, reset and try again.
    if(!removeRandomValue(cells)) {
      memset(cells, 0, sizeof(int) * TOTAL_CELLS);
      fillPuzzleRandomly(cells);
      continue;
    }
    cellsRemovedCount++;
    *difficultyScore = determineDifficulty(cells);

    if(*difficultyScore < minDifficultyScore || cellsRemovedCount < MIN_CELLS_REMOVED) {
      continue;
    }
    if(*difficultyScore >= maxDifficultyScore) {
      // reset because the score is greater than the maxDifficulty score.
      memset(cells, 0, sizeof(int) * TOTAL_CELLS);
      fillPuzzleRandomly(cells);
      cellsRemovedCount = 0;
    } else {
      puzzleAtDifficulty = true;
    }
  }

}

int determineDifficulty(int* cells) {
  StepNode* stepList = initStepList();
  Puzzle puzzle = {
    .candidates = {0},
    .id = 0
  };
  memcpy(puzzle.cells, cells, sizeof(int) * TOTAL_CELLS);
  solvePuzzle(&puzzle, stepList);
  int totalScore = 0;
  StepSummary summary = {0};
  getStepSummary(&summary, stepList);
  freeStepList(stepList);
  return summary.difficultyScore;
}

// Returns true if successfull and false if no more values can be removed without introducing another possible solution.
bool removeRandomValue(int* cells) {
  int filledCellIndicies[81] = {-1};
  int filledCount = 0;
  bool removed = false;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    if(cells[i] != 0) {
      filledCellIndicies[filledCount] = i;
      filledCount++;
    }
  }
  shuffleIntArr(filledCellIndicies, filledCount);
  if(filledCount < 1) {
    return false;
  }
  for(int i = 0; i < filledCount; ++i) {
    int backtrackValue = cells[filledCellIndicies[i]];
    cells[filledCellIndicies[i]] = 0;
    if(!isUnique(cells)) {
      cells[filledCellIndicies[i]] = backtrackValue;
    } else {
      return true;
    }
  }

}

bool fillPuzzleRandomly(int* cells) {
  int emptyCellIndex = findEmptyCell(cells);
  if(emptyCellIndex == -1) {
    return true;
  }
  int potentialValues[9] = {1,2,3,4,5,6,7,8,9};
  shuffleIntArr(potentialValues, PUZZLE_WIDTH);

  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int rowIndex = emptyCellIndex / PUZZLE_WIDTH;
    int colIndex = emptyCellIndex % PUZZLE_WIDTH;
    if(numWorksInCell(rowIndex, colIndex, potentialValues[i], cells)) {
      cells[emptyCellIndex] = potentialValues[i];
      // recursively solve
      if(fillPuzzleRandomly(cells)) {
        return true;
      }
      cells[emptyCellIndex] = 0;
    }
  }
  return false;
}

bool isUnique(int* cells) {
  int cpy[81];
  memcpy(cpy, cells, sizeof(int) * 81);
  int solutionCount = 0;
  countSolutionsRecursive(cells, &solutionCount);

  if(solutionCount > 1) {
    return false;
  } else {
    return true;
  }
}

void countSolutionsRecursive(int* cells, int* solutionCount) {
  if(*solutionCount > 1) {
    return;
  }
  int emptyCellIndex = findEmptyCell(cells);
  if(emptyCellIndex == -1) {
    *solutionCount++;
    return;
  }

  for(int i = 1; i <= PUZZLE_WIDTH; ++i) {
    int rowIndex = emptyCellIndex / PUZZLE_WIDTH;
    int colIndex = emptyCellIndex % PUZZLE_WIDTH;
    if(numWorksInCell(rowIndex, colIndex, i, cells)) {
      cells[emptyCellIndex] = i;
      countSolutionsRecursive(cells, solutionCount);
      if(*solutionCount > 1) return;
      // backtrack to check other values;
      cells[emptyCellIndex] = 0;
    } 
  }  
}

void setMinMaxDifficulty(int* min, int* max, DifficultyRating targetDifficulty) {
  switch (targetDifficulty)
  {
  case BEGINNER:
    *min = 0;
    *max = 150;
    break;
  case EASY:
    *min = 150;
    *max = 250;
    break;
  case MEDIUM:
    *min = 250;
    *max = 500;
    break;
  case HARD:
    *min = 500;
    *max = 100;
    break;
  case IMPOSSIBLE:
    *min = 1000;
    *max = INT32_MAX;
    break;
  default:
    break;
  }
}

int parsePuzzleCount(char* arg) {
  int base = 10;
  int errnum = 0;
  char* endptr;
  int count = strtol(arg, &endptr, base);
  if (endptr == arg) {
    fprintf(stderr, "Error: No digits were found in argument '%s'\n", arg);
    return -1;
  } else if (*endptr != '\0') {
    fprintf(stderr, "Error: Trailing characters after number in argument '%s'\n", arg);
    return -1;
  } else if (errnum == ERANGE) {
    fprintf(stderr, "Error: Number out of range for long int in argument '%s'\n", arg);
    return -1;
  }
  return count;
}

DifficultyRating parseDifficultyArg(char* arg) {
  if(!strcmp("BEGINNER", arg)) {
    return BEGINNER;
  } else if (!strcmp("EASY", arg)) {
    return EASY;
  } else if (!strcmp("MEDIUM", arg)) {
    return MEDIUM;
  } else if (!strcmp("HARD", arg)) {
    return HARD;
  } else if (!strcmp("IMPOSSIBLE", arg)) {
    return IMPOSSIBLE;
  } else {
    fprintf(stderr, "Error: Difficulty argurment is incorrect: '%s' not recognized.", arg);
    return -1;
  }
}

void formatCellsToString(int* cells, char* string) {
  int i;
  for(i = 0; i < TOTAL_CELLS; ++i) {
    string[i] = cells[i] + '0'; // int to ascii conversion
  }
  string[i] = '\0';
}