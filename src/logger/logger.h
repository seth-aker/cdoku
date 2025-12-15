#ifndef SRC_LOGGER_LOGGER_H
#define SRC_LOGGER_LOGGER_H

#include "step.h"

void printRow(int rowNum, int* cells) ;
void printCells(int* cells);

void printSummary(StepSummary* summary);

void printStep(Step step, int stepNumber);

#endif // SRC_LOGGER_LOGGER_H