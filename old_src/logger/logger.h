#ifndef SRC_LOGGER_LOGGER_H
#define SRC_LOGGER_LOGGER_H

#include "step.h"

#define LOG_LEVEL LOG_INFO

void setupLogger();

void printRow(int rowNum, int* cells, char* dest) ;
void printCells(int* cells);

void printSummary(StepSummary* summary);

void printStep(Step step, int stepNumber);

#endif // SRC_LOGGER_LOGGER_H