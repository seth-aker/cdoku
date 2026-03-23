#ifndef SRC_GENERATOR_GENERATE_PUZZLE_H
#define SRC_GENERATOR_GENERATE_PUZZLE_H
#include "puzzle.h"
#include "step.h"

void generatePuzzle(DiffRating targetDifficulty, int* cells, int* difficultyScore);

int determineDifficulty(int* cells);

bool removeRandomValue(int* cells);

bool fillPuzzleRandomly(int* cells);

bool isUnique(int* cells);

void countSolutionsRecursive(int* cells, int* solutionCount);

void setMinMaxDifficulty(int* min, int* max, DiffRating targetDifficulty);

int parsePuzzleCount(char* arg);

DiffRating parseDifficultyArg(char* arg);

void formatCellsToString(int* cells, char* string);



#endif // SRC_GENERATOR_GENERATE_PUZZLE_H
