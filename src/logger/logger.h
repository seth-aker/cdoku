#ifndef SRC_LOGGER_LOGGER_H
#define SRC_LOGGER_LOGGER_H

#include "solver/strategies.h"
#include "solver/sudoku_solver.h"
#include "solver/step.h"

void printRow(int rowNum, int* cells) ;
void printCells(int* cells);

void printStep(Step step);

#endif // SRC_LOGGER_LOGGER_H