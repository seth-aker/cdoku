#include "generate_puzzle.h"
#include "stdio.h"
#include "string.h"
#include "utils.h"
#include <errno.h>

int main(int argc, char* argv[]) {
  if(argc != 3) {
    perror("Incorrect number of arguments\n");
    return 1;
  }
  int puzzleCount = parsePuzzleCount(argv[1]);
  if(puzzleCount == -1) {
    return 1;
  }
  int difficulty = parseDifficultyArg(argv[2]);
  if(difficulty == -1) {
    return 1;
  }

  int cells[81] = {0};
  int difficultyScore = 0;
  char* puzzleString[81];
  for(int i = 0; i < puzzleCount; ++i) {
    generatePuzzle(difficulty, &cells, &difficultyScore);
    puzzleString[0] = "\0";
    formatCellsToString(cells, puzzleString);
    // this prints to node in the following format
    // [cellValues],[difficultyScore];
    // e.g. 000000000...x81 numbers...000,12345;
    printf("%s,%d;\n", puzzleString, difficultyScore);
  }
}

void generatePuzzle(int targetDifficulty, int* cells, int* difficultyScore) {
  int minDifficultyScore = 0;
  int maxDifficultyScore = 0;
  fillPuzzleRandomly(cells);
  
  
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

int findEmptyCell(int* cells) {
  for(int i = 0; i < 81; ++i) {
    if(cells[i] == 0) return i;
  }
  return -1;
}

int parsePuzzleCount(char* arg) {
  int base = 10;
  int errnum = 0;
  char* endptr;
  int count = strtol(arg, &endptr, base);
  if (endptr == arg) {
    printf(stderr, "Error: No digits were found in argument '%s'\n", arg);
    return -1;
  } else if (*endptr != '\\0') {
    printf(stderr, "Error: Trailing characters after number in argument '%s'\n", arg);
    return -1;
  } else if (errnum == ERANGE) {
    printf(stderr, "Error: Number out of range for long int in argument '%s'\n", arg);
    return -1;
  }
}

int parseDifficultyArg(char* arg) {
  if(strcmp("BEGINNER", arg)) {
    return BEGINNER;
  } else if (strcmp("EASY", arg)) {
    return EASY;
  } else if (strcmp("MEDIUM", arg)) {
    return MEDIUM;
  } else if (strcmp("HARD", arg)) {
    return HARD;
  } else if (strcmp("IMPOSSIBLE", arg)) {
    return IMPOSSIBLE;
  } else {
    printf("Error: Difficulty argurment is incorrect: '%s' not recognized.", arg);
    return -1;
  }
}

void formatCellsToString(int* cells, char* string) {
  char cellValueChar;
  for(int i = 0; i < 81; i++) {
    sprintf(&cellValueChar, "%d", cells[i]);
    strcat(string, cellValueChar);
  }
}