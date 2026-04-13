#include "xy_wing_solver.h"

StepNode* solveXYWing(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  XYWingContext context;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    uint16_t candidate = puzzle->candidates[i];
    if(candidate == 0 || __builtin_popcount(candidate) != 2) {
      continue;
    }
    context.pivot = candidate;
    context.pivotIndex = i;
    if(findXYWing(puzzle, &context, current)) {
      return current;
    }
    
  }
  return current;
}

bool findXYWing(Puzzle* puzzle, XYWingContext* context, StepNode* head) {
  StepNode* current = head;
  House rowHouse = {.type = ROW};
  House colHouse = {.type = COL};
  House blockHouse = {.type = BLOCK};
  context->pBlockX = calcBlockX(context->pivotIndex);
  context->pBlockY = calcBlockY(context->pivotIndex);
  rowHouse.index = context->pivotIndex / PUZZLE_WIDTH;
  colHouse.index = context->pivotIndex % PUZZLE_WIDTH;
  blockHouse.index = context->pBlockX + context->pBlockY * BLOCK_WIDTH;
  getCandidateRow(context->pRowIndex, puzzle->candidates, rowHouse.candidates);
  getCandidateCol(context->pColIndex, puzzle->candidates, colHouse.candidates);
  getCandidateBlock(context->pBlockX, context->pBlockY, puzzle->candidates, blockHouse.candidates);

  int rowPotentials[PUZZLE_WIDTH];
  int rowPotentialsCount = getHousePotentials(&rowHouse, rowPotentials, context->pivotIndex, context->pivot);

  int colPotentials[PUZZLE_WIDTH];
  int colPotentialsCount = getHousePotentials(&colHouse, colPotentials, context->pivotIndex, context->pivot);

  int blockPotentials[PUZZLE_WIDTH];
  int blockPotentialsCount = getHousePotentials(&blockHouse, blockPotentials, context->pivotIndex, context->pivot);

  if((rowPotentialsCount + colPotentialsCount + blockPotentialsCount) < 2) return false;

  // row x col
  context->houseX = &rowHouse;
  context->xPotentialIndicies = &rowPotentials;
  context->xLen = rowPotentialsCount;
  context->houseY = &colHouse;
  context->yPotentialIndicies = &colPotentials;
  context->yLen = colPotentialsCount;
  
  if(compareHousePotentials(puzzle, context)) {
    current = eliminateXYIntersections(puzzle, context, current);
  }
  if(current != head) return true;


  // row X block
  context->houseY = &blockHouse;
  context->yPotentialIndicies = &blockPotentials;
  context->yLen = blockPotentialsCount;
  
  if(compareHousePotentials(puzzle, context)) {
    current = eliminateXYIntersections(puzzle, context, current);
  }
  if(current != head) return true;

  // col x block
  context->houseX = &colHouse;
  context->xPotentialIndicies = &colPotentials;
  context->xLen = colPotentialsCount;
  
  if(compareHousePotentials(puzzle, context)) {
    current = eliminateXYIntersections(puzzle, context, current);
  }
  if(current != head) return true;

}

int getHousePotentials(House* house, int* potentials, int pivotIndex, uint16_t pivot) {
  int potentialCount = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i == pivotIndex) continue;

    if(__builtin_popcount(house->candidates[i]) != 2) continue;

    uint16_t candidateUnion = house->candidates[i] & pivot;
    if(__builtin_popcount(candidateUnion) == 1) {
      potentials[potentialCount] = getCellIndexFromHousePos(house, i);
      potentialCount++;
    };
  }
  return potentialCount;
}

bool compareHousePotentials(Puzzle* puzzle, XYWingContext* context) {
  for(int x = 0; x < context->xLen; ++x) {
    int xIndex = context->xPotentialIndicies[x];
    for(int y = 0; y < context->yLen; ++y ) {
      int yIndex = context->yPotentialIndicies[y];
      uint16_t zValue = context->pivot & ~puzzle->candidates[xIndex];
      if(zValue == context->pivot & ~puzzle->candidates[yIndex]) {
        context->xIndex = xIndex;
        context->yIndex = yIndex;
        context->zValue = zValue;
        return true;
      }
    }
  }
  return false;
}

StepNode* eliminateXYIntersections(Puzzle* puzzle, XYWingContext* context, StepNode* head) {
  StepNode* current = head;
  int intersectionIndicies[5]; // 5 is the max there can be in an XY Wing 
  int intersectionIndiciesCount = 0;
  int xRowIndex = context->xIndex / PUZZLE_WIDTH;
  int yRowIndex = context->yIndex / PUZZLE_WIDTH;
  int xColIndex = context->xIndex % PUZZLE_WIDTH;
  int yColIndex = context->yIndex % PUZZLE_WIDTH;
  
  int xBlockCol = calcBlockX(context->xIndex);
  int yBlockCol = calcBlockX(context->yIndex);
  int xBlockRow = calcBlockY(context->xIndex);
  int yBlockRow = calcBlockY(context->yIndex);
  
  if(xBlockCol == yBlockCol && xBlockCol == context->pBlockX) { // all elements in the same block-col
    if(context->pColIndex == xColIndex) { // if x is in the same col as pivot then y is in the same block as pivot, get elements in pivots col/block and in x's col/block
      int rowStart = context->pBlockY * BLOCK_WIDTH;
      for(int i = rowStart; i < rowStart + BLOCK_WIDTH; ++i) {
        int index = xColIndex + i * PUZZLE_WIDTH;
        if(index == context->pivotIndex) continue;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }

      rowStart = yBlockRow * BLOCK_WIDTH;
      for(int i = rowStart; i < rowStart + BLOCK_WIDTH; ++i) {
        int index = yColIndex +  i * PUZZLE_WIDTH;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }
    } else {
      int rowStart = context->pBlockY * BLOCK_WIDTH;
      for(int i = rowStart; i < rowStart + BLOCK_WIDTH; ++i) {
        int index = yColIndex + i * PUZZLE_WIDTH;
        if(index == context->pivotIndex) continue;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }

      rowStart = xBlockRow * BLOCK_WIDTH;
      for(int i = rowStart; i < rowStart + BLOCK_WIDTH; ++i) {
        int index = xColIndex + i * PUZZLE_WIDTH;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }
    }
  } else if (xBlockRow == yBlockRow && xBlockRow == context->pBlockY) { // all elements in the same block-row
    if(context->pRowIndex == xRowIndex) { // if x is in the same row as pivot then y is in the same block as pivot, get elements in pivots row/block and in x's row/block
      int colStart = context->pBlockX * BLOCK_WIDTH;
      for(int i = colStart; i < colStart + BLOCK_WIDTH; ++i) {
        int index = i + xRowIndex * PUZZLE_WIDTH;
        if(index == context->pivotIndex) continue;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }

      colStart = xBlockCol * BLOCK_WIDTH;
      for(int i = colStart; i < colStart + BLOCK_WIDTH; ++i) {
        int index = i + yRowIndex * PUZZLE_WIDTH;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }

    } else {
      int colStart = context->pBlockX * BLOCK_WIDTH;
      for(int i = colStart; i < colStart + BLOCK_WIDTH; ++i) {
        int index = i + yRowIndex * PUZZLE_WIDTH;
        if(index == context->pivotIndex) continue;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }

      colStart = yBlockCol * BLOCK_WIDTH;
      for(int i = colStart; i < colStart + BLOCK_WIDTH; ++i) {
        int index = i + xRowIndex * PUZZLE_WIDTH;
        if(puzzle->candidates[index] == 0) continue;

        intersectionIndicies[intersectionIndiciesCount] = index;
        intersectionIndiciesCount++;
      }
    }
  } else {
    // get the x by y intersection
    if(xRowIndex == context->pRowIndex) {
      intersectionIndicies[intersectionIndiciesCount] = xColIndex + (yRowIndex * PUZZLE_WIDTH);
      intersectionIndiciesCount++;
    } else {
      intersectionIndicies[intersectionIndiciesCount] = yColIndex + (xRowIndex * PUZZLE_WIDTH);
      intersectionIndiciesCount++;
    }
  }


  for(int i = 0; i < intersectionIndiciesCount; i++) {
    uint16_t* cellCandidates = &puzzle->candidates[intersectionIndicies[i]];
    uint16_t valsRemoved = removeCandidates(cellCandidates, context->zValue);
    if(valsRemoved != 0) {
      int colIndex = intersectionIndicies[i] % PUZZLE_WIDTH;
      int rowIndex = intersectionIndicies[i] / PUZZLE_WIDTH;
      Step newStep = {
        .candidatesRemoved = valsRemoved,
        .colIndex = colIndex,
        .rowIndex = rowIndex,
        .strategyUsed = XY_WING,
        .value = 0
      };
      current = appendStep(current, newStep);
    }
  }
  return current;
}
