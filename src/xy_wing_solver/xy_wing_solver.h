#ifndef SRC_XY_WING_SOLVER_XY_WING_SOLVER_H
#define SRC_XY_WING_SOLVER_XY_WING_SOLVER_H

#include <stdbool.h>
#include "puzzle.h"
#include "step.h"

typedef struct {
  House* houseX;
  House* houseY;
  int* xPotentialIndicies;
  int xLen;
  int* yPotentialIndicies;
  int yLen;
  int pivotIndex;
  uint16_t pivot;
  int pRowIndex;
  int pColIndex;
  int pBlockX;
  int pBlockY;
  int xIndex;
  int yIndex;
  uint16_t zValue;
} XYWingContext;
StepNode* solveXYWing(Puzzle* puzzle, StepNode* head);
  

bool findXYWing(Puzzle* puzzle, XYWingContext* context, StepNode* head);

int getHousePotentials(House* house, int* potentials, int pivotIndex, uint16_t pivot);
bool compareHousePotentials(Puzzle* puzzle, XYWingContext* context);
StepNode* eliminateXYIntersections(Puzzle* puzzle, XYWingContext* context, StepNode* head);

#endif