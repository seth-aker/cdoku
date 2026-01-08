#include "sudoku_solver.h"

bool solvePuzzle(Puzzle* puzzle, StepNode* head) {
  if(head == NULL) {
    return false;
  }
 
  fillPuzzleCandidates(puzzle);
  return solveRecursive(puzzle, head);
}
bool solveRecursive(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  while(!isPuzzleSolved(puzzle->cells)) {
    // look for a single and if one exists adds it and updates current.
    // Find single returns the updated node if a single was found OR the same node that was passed.
    StepNode* newStep = findSingles(puzzle, current);
    if(current != newStep) {
      current = newStep;
      continue;
    }
    newStep = findLockedCandidates(puzzle, current);
    if(current != newStep) {
      current = newStep;
      // Puzzle has been modified and we need to check if it can be solved/go through the loop again.
      continue;
    }

    newStep = findSubsets(puzzle, current);
    if(current != newStep) {
      current = newStep;
      continue;
    }
    newStep = findBasicFish(puzzle, current);
    if(current != newStep) {
      current = newStep;
      continue;
    }
    newStep = findFinnedFish(puzzle, current);
    if(current != newStep) {
      current = newStep;
      continue;
    }
    
    if(makeGuess(puzzle, current)) {
      return true;
    }
    return false;
  }
  return true;
}
bool isPuzzleSolved(int* cells) {
  for(int rowIndex = 0; rowIndex < PUZZLE_WIDTH; ++rowIndex) {
    for(int colIndex = 0; colIndex < PUZZLE_WIDTH; ++colIndex) {
      int cellIndex = (rowIndex * PUZZLE_WIDTH) + colIndex;
      int cellValue = cells[cellIndex];
      if(cellValue == 0 || !numWorksInCell(rowIndex, colIndex, cellValue, cells)) {
        return false;
      }
    }
  }
  return true;
}


StepNode* findSingles(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findFullHouses(puzzle, current);
  if(current != head) {
    return current;
  }
  current = findNakedSingles(puzzle, current);
  if(current != head) {
    return current;
  }
  current = findHiddenSingles(puzzle, current);
  return current;
}
StepNode* findNakedSingles(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  for(int cellIndex = 0; cellIndex < TOTAL_CELLS; ++cellIndex) {
    if(puzzle->cells[cellIndex] != 0) {
      continue;
    }
    int count = __builtin_popcount(puzzle->candidates[cellIndex]);
    if(count == 1) {
      int value = getFirstValueFromMask(puzzle->candidates[cellIndex]);
      current = applyFoundValue(puzzle, value, cellIndex, 
        isFullHouse(cellIndex, puzzle->cells) ? FULL_HOUSE : NAKED_SINGLE,
        current
      );
      return current;
    }
  }
  return current;
}
StepNode* findFullHouses(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  for(int cellIndex = 0; cellIndex < TOTAL_CELLS; ++cellIndex) {
    if(puzzle->cells[cellIndex] != 0) {
      continue;
    }
    int count = __builtin_popcount(puzzle->candidates[cellIndex]);
    if(count == 1 && isFullHouse(cellIndex, puzzle->cells)) {
      int value = getFirstValueFromMask(puzzle->candidates[cellIndex]);
      current = applyFoundValue(puzzle, value, cellIndex, FULL_HOUSE, current);
      return current;
    }
  }
  return current;
}
StepNode* findHiddenSingles(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  for(int cellIndex = 0; cellIndex < TOTAL_CELLS; ++cellIndex) {
    if(puzzle->cells[cellIndex] != 0) {
      continue;
    }
    int rowIndex = cellIndex / PUZZLE_WIDTH;
    int colIndex = cellIndex % PUZZLE_WIDTH;
    int rowStart = rowIndex * PUZZLE_WIDTH;
    
    uint16_t cell = puzzle->candidates[cellIndex];
  
    uint16_t others = 0;
    // Check the row;
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      if(i == colIndex) continue;
      others |= puzzle->candidates[rowStart + i];
    }
    uint16_t unique = cell & ~others;
    if(unique) {
      // TODO: refactor to function
      int value = getFirstValueFromMask(unique);
      current = applyFoundValue(puzzle, value, cellIndex, HIDDEN_SINGLE, current);
      return current;
    }
    others = 0;
  
    // Check the col;
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      if(i == rowIndex) continue;
      others |= puzzle->candidates[i * PUZZLE_WIDTH + colIndex];
    }
    unique = cell & ~others;
    if(unique) {
      int value = getFirstValueFromMask(unique);
      current = applyFoundValue(puzzle, value, cellIndex, HIDDEN_SINGLE, current);
      return current;
    }
    
    others = 0;
    //Check the block
    uint16_t block[9];
    getCandidateBlock(colIndex / BLOCK_WIDTH, rowIndex / BLOCK_WIDTH, puzzle->candidates, block);
    int cellPosition = getCellPosInBlock(rowIndex, colIndex);
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      if(i == cellPosition) continue;
      others |= block[i];
    }
    unique = cell & ~others;
    if(unique) {
      int value = getFirstValueFromMask(unique);
      current = applyFoundValue(puzzle, value, cellIndex, HIDDEN_SINGLE, current);
      return current;
    }
  }
  return current;
}

StepNode* findLockedCandidates(Puzzle* puzzle, StepNode* head) {
  StepNode* newStep = findLockedCandidatePointing(puzzle, head);
  if(head != newStep) {
    return newStep;
  }
  newStep = findLockedCandidateClaiming(puzzle, head);
  if(head != newStep) {
    return newStep;
  }
  return head;
}

StepNode* findLockedCandidatePointing(Puzzle* puzzle, StepNode* head) {
  uint16_t candidateBlock[9];

  for(int blockX = 0; blockX < BLOCK_WIDTH; ++blockX) {
    for(int blockY = 0; blockY < BLOCK_WIDTH; ++blockY) {
      getCandidateBlock(blockX, blockY, puzzle->candidates, candidateBlock);
      uint16_t row1 = candidateBlock[0] | candidateBlock[1] | candidateBlock[2];
      uint16_t row2 = candidateBlock[3] | candidateBlock[4] | candidateBlock[5];
      uint16_t row3 = candidateBlock[6] | candidateBlock[7] | candidateBlock[8];

      // candidates unique to row1
      uint16_t uRow1 = row1 & ~(row2 | row3);
      uint16_t uRow2 = row2 & ~(row1 | row3);
      uint16_t uRow3 = row3 & ~(row1 | row2);

      if(uRow1) {
        StepNode* lastChange = removePointingRow(blockY * BLOCK_WIDTH, blockX * BLOCK_WIDTH, uRow1, puzzle, head);
        if(lastChange != head) {
          return lastChange;
        }
      }
      if(uRow2) {
        StepNode* lastChange = removePointingRow(blockY * BLOCK_WIDTH + 1, blockX * BLOCK_WIDTH, uRow2, puzzle, head);
        if(lastChange != head) {
          return lastChange;
        }
      }
      if(uRow3) {
        StepNode* lastChange = removePointingRow(blockY * BLOCK_WIDTH + 2, blockX * BLOCK_WIDTH, uRow3, puzzle, head);
        if(lastChange != head) {
          return lastChange;
        }
      }
      uint16_t col1 = candidateBlock[0] | candidateBlock[3] | candidateBlock[6];
      uint16_t col2 = candidateBlock[1] | candidateBlock[4] | candidateBlock[7];
      uint16_t col3 = candidateBlock[2] | candidateBlock[5] | candidateBlock[8];

      // Unique candidates in columns;
      uint16_t uCol1 = col1 & ~(col2 | col3);
      uint16_t uCol2 = col2 & ~(col1 | col3);
      uint16_t uCol3 = col3 & ~(col1 | col2);

      if(uCol1) {
        StepNode* lastChange = removePointingCol(blockX * BLOCK_WIDTH, blockY * BLOCK_WIDTH, uCol1, puzzle, head);
        if(lastChange == head) {
          return lastChange;
        }
      }
      if(uCol2) {
        StepNode* lastChange = removePointingCol(blockX * BLOCK_WIDTH + 1, blockY * BLOCK_WIDTH, uCol2, puzzle, head);
        if(lastChange != head) {
          return lastChange;
        }
      }
      if(uCol3) {
        StepNode* lastChange = removePointingCol(blockX * BLOCK_WIDTH + 2, blockY * BLOCK_WIDTH, uCol3, puzzle, head);
        if(lastChange != head) {
          return lastChange;
        }
      }
    }
  }
  return head;
}
StepNode* findLockedCandidateClaiming(Puzzle* puzzle, StepNode* head) {
  uint16_t candidateRow[9];
  for(int rowIndex = 0; rowIndex < PUZZLE_WIDTH; ++rowIndex) {
    getCandidateRow(rowIndex, puzzle->candidates, candidateRow);
    uint16_t rowSeg1 = candidateRow[0] | candidateRow[1] | candidateRow[2];
    uint16_t rowSeg2 = candidateRow[3] | candidateRow[4] | candidateRow[5];
    uint16_t rowSeg3 = candidateRow[6] | candidateRow[7] | candidateRow[8];

    uint16_t uRowSeg1 = rowSeg1 & ~(rowSeg2 | rowSeg3);
    uint16_t uRowSeg2 = rowSeg2 & ~(rowSeg1 | rowSeg3);
    uint16_t uRowSeg3 = rowSeg3 & ~(rowSeg1 | rowSeg2);

    int blockOffset = (rowIndex / BLOCK_WIDTH) * BLOCK_WIDTH;

    if(uRowSeg1) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uRowSeg1, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        BlockCoord coords = {
          .blockX = 0,
          .blockY = rowIndex / BLOCK_WIDTH
        };
        StepNode* lastStep = removeCandidateFromBlock(coords, candidatesToRemove[i], rowIndex % BLOCK_WIDTH, -1, LOCKED_CANDIDATE_POINTING, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uRowSeg2) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uRowSeg2, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        BlockCoord coords = {
          .blockX = 1,
          .blockY = rowIndex / BLOCK_WIDTH
        };
        StepNode* lastStep = removeCandidateFromBlock(coords, candidatesToRemove[i], rowIndex % BLOCK_WIDTH, -1, LOCKED_CANDIDATE_POINTING, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uRowSeg3) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uRowSeg3, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        BlockCoord coords = {
          .blockX = 2,
          .blockY = rowIndex / BLOCK_WIDTH
        };
        StepNode* lastStep = removeCandidateFromBlock(coords, candidatesToRemove[i], rowIndex % BLOCK_WIDTH, -1, LOCKED_CANDIDATE_POINTING, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
  }
  uint16_t candidateCol[9] = {0};
  for(int colIndex = 0; colIndex < PUZZLE_WIDTH; ++colIndex) {
    getCandidateCol(colIndex, puzzle->candidates, candidateCol);
    uint16_t colSeg1 = candidateCol[0] | candidateCol[1] | candidateCol[2];
    uint16_t colSeg2 = candidateCol[3] | candidateCol[4] | candidateCol[5];
    uint16_t colSeg3 = candidateCol[6] | candidateCol[7] | candidateCol[8];

    uint16_t uColSeg1 = colSeg1 & ~(colSeg2 | colSeg3);
    uint16_t uColSeg2 = colSeg2 & ~(colSeg1 | colSeg3);
    uint16_t uColSeg3 = colSeg3 & ~(colSeg1 | colSeg2);

    if(uColSeg1) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uColSeg1, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        BlockCoord coords = {
          .blockX = colIndex / BLOCK_WIDTH,
          .blockY = 0
        };
        StepNode* lastStep = removeCandidateFromBlock(coords, candidatesToRemove[i], -1, colIndex % BLOCK_WIDTH, LOCKED_CANDIDATE_CLAIMING, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uColSeg2) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uColSeg2, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        BlockCoord coords = {
          .blockX = colIndex / BLOCK_WIDTH,
          .blockY = 1
        };
        StepNode* lastStep = removeCandidateFromBlock(coords, candidatesToRemove[i], -1, colIndex % BLOCK_WIDTH, LOCKED_CANDIDATE_CLAIMING, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uColSeg3) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uColSeg3, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        BlockCoord coords = {
          .blockX = colIndex / BLOCK_WIDTH,
          .blockY = 2
        };
        StepNode* lastStep = removeCandidateFromBlock(coords, candidatesToRemove[i], -1, colIndex % BLOCK_WIDTH, LOCKED_CANDIDATE_CLAIMING, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
  }
  return head;
}

StepNode* findSubsets(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  for(int subsetSize = 2; subsetSize <= 4; ++subsetSize) {
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      House house;
      house.index = i;
      
      // ROW
      getCandidateRow(i, puzzle->candidates, house.candidates);
      getRow(i, puzzle->cells, house.cells);
      house.type = ROW;
      
      current = findNakedSubsetOfSize(puzzle, &house, subsetSize, current);
      if(current != head) {
        return current;
      }
      
      // COL
      getCandidateCol(i, puzzle->candidates, house.candidates);
      getCol(i, puzzle->cells, house.cells);
      house.type = COL;
      
      current = findNakedSubsetOfSize(puzzle, &house, subsetSize, current);
      if(current != head) {
        return current;
      }
      
      // BLOCK
      int blockX = i % BLOCK_WIDTH;
      int blockY = i / BLOCK_WIDTH;
      getCandidateBlock(blockX, blockY, puzzle->candidates, house.candidates);
      getBlock(blockX, blockY, puzzle->cells, house.cells);
      house.type = BLOCK;
      
      current = findNakedSubsetOfSize(puzzle, &house, subsetSize, current);
      if(current != head) {
        return current;
      }
    }
  }
  for(int subsetSize = 2; subsetSize <= 4; ++subsetSize) {
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      House house;
      house.index = i;

      getCandidateRow(i, puzzle->candidates, house.candidates);
      getRow(i, puzzle->cells, house.cells);
      house.type = ROW;

      current = findHiddenSubsetOfSize(puzzle, &house, subsetSize, current);
      if(current != head) {
        return current;
      }

      getCandidateCol(i, puzzle->candidates, house.candidates);
      getCol(i, puzzle->cells, house.cells);
      house.type = COL;
      current = findHiddenSubsetOfSize(puzzle, &house, subsetSize, current);
      if(current != head) {
        return current;
      }

      int blockX = i % BLOCK_WIDTH;
      int blockY = i / BLOCK_WIDTH;
      getCandidateBlock( blockX, blockY, puzzle->candidates, house.candidates);
      getBlock(blockX, blockY, puzzle->cells, house.cells);
      house.type = BLOCK;
      current = findHiddenSubsetOfSize(puzzle, &house, subsetSize, current);
      if(current != head) {
        return current;
      }
    }
  }
  return current;
}

StepNode* findNakedSubsetOfSize(Puzzle* puzzle, House* house, int subsetSize, StepNode* head) {
  
  NakedComboSearchContext context;
  int foundBuffer[subsetSize];
  int subsetBuffer[subsetSize];
  context.candidates = house->candidates;
  context.values = house->cells;
  context.subsetCandidates = 0;
  context.foundIndicies = foundBuffer;
  context.subsetIndicies = subsetBuffer;

  if(findNakedCombo(&context, 0, subsetSize, 0)) {
    StepNode* newStep = removeNakedSubsetFromHouse(puzzle, &context, house, subsetSize, head);
    if(newStep != head) {
      return newStep;
    }
  }
  return head;
}

StepNode* removeNakedSubsetFromHouse(Puzzle* puzzle, NakedComboSearchContext* context, House* house, int subsetSize, StepNode* head) {
  StepNode* current = head;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    bool isSubset = false;
    for(int k = 0; k < subsetSize; ++k) {
      if(i == context->foundIndicies[k]) {
        isSubset = true;
      }
    }
    if(!isSubset && (house->candidates[i] & context->subsetCandidates)) {
      int candidates[9] = {0};
      int candidateCount = getCandidatesInCell(context->subsetCandidates, candidates);
      int cellLocation = getCellIndexFromHousePos(house, i);
      // Check if candidates will be removed and store them here.
      uint16_t candidatesRemoved = puzzle->candidates[cellLocation] & context->subsetCandidates;
      // accutally remove them.
      puzzle->candidates[cellLocation] &= ~context->subsetCandidates;
      // if no candidates were removed this will be 0
      if(candidatesRemoved) {
        Step newStep;
        newStep.candidatesRemoved = candidatesRemoved;
        switch(subsetSize) {
          case 2:
          newStep.strategyUsed = NAKED_PAIRS;
          break;
          case 3:
          newStep.strategyUsed = NAKED_TRIPLES;
          break;
          case 4: 
          newStep.strategyUsed = NAKED_QUADS;
        }
        newStep.value = candidates[i];
        switch (house->type) {
          case ROW:
          newStep.rowIndex = house->index;
          newStep.colIndex = i;
          break;
          case COL: 
          newStep.rowIndex = i;
          newStep.colIndex = house->index;
          break;
          default: // BLOCK
          newStep.rowIndex = ((house->index / PUZZLE_WIDTH) * BLOCK_WIDTH) + (i / PUZZLE_WIDTH);
          newStep.colIndex = (house->index % PUZZLE_WIDTH * BLOCK_WIDTH) + (i / PUZZLE_WIDTH);
          break;
        }
        current = appendStep(current, newStep);
      }
    }
  }
  return current;
}

bool findNakedCombo(NakedComboSearchContext* context, int startIndex, int subsetSize, int depth) {
  if(depth == subsetSize) {
    uint16_t unionMask = 0;
    for(int i = 0; i < subsetSize; ++i) {
      int cellIndex = context->subsetIndicies[i];
      unionMask |= context->candidates[cellIndex];
    }
    if(__builtin_popcount(unionMask) == subsetSize) {
      for(int i = 0; i < subsetSize; ++i) {
        context->foundIndicies[i] = context->subsetIndicies[i];
      }
      context->subsetCandidates = unionMask;
      return true;
    }
    return false;
  }
  for(int i = startIndex; i < PUZZLE_WIDTH; ++i) {
    if(context->values[i] > 0) continue;

    if(__builtin_popcount(context->candidates[i]) > subsetSize) continue;

    if(includes(context->subsetIndicies, depth, i)) continue;

    context->subsetIndicies[depth] = i;
    bool combinationFound = findNakedCombo(context, startIndex + 1, subsetSize, depth + 1);
    if(combinationFound) {
      return true;
    }
  }
  return false;
}

StepNode* findHiddenSubsetOfSize(Puzzle* puzzle, House* house, int subsetSize, StepNode* head) {
  HiddenComboSearchContext context; 
  context.cells = house->cells;
  context.candidates = house->candidates;
  context.emptyCellCount = 0;
  int hiddenComboCellIndicies[9] = {0};
  context.hiddenComboCellIndices = hiddenComboCellIndicies;
  uint16_t allCandidateUnion = 0;
  int emptyCellBuffer[9];
  
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house->cells[i] == 0) {
      emptyCellBuffer[context.emptyCellCount] = i;
      context.emptyCellCount++;
      allCandidateUnion |= house->candidates[i];
    }
  }
  context.emptyCellIndicies = emptyCellBuffer;
  int candidateBuffer[9];
  context.allCandidateCount = getCandidatesInCell(allCandidateUnion, candidateBuffer);
  context.allCandidateArray = candidateBuffer;
  uint16_t candidateSubsetBuffer[4] = {0}; // MAX DEPTH
  context.candidateSubset = candidateSubsetBuffer;

  if(findHiddenCombo(&context, 0, subsetSize, 0)) {
    StepNode* newStep = removeHiddenSubsetFromHouse(puzzle, house, &context, subsetSize, head);
    if(newStep != head) {
      return newStep;
    }
  }
  return head;

}
StepNode* removeHiddenSubsetFromHouse(Puzzle* puzzle, House* house, HiddenComboSearchContext* context, int subsetSize, StepNode* head) {
  StepNode* current = head;
  uint16_t comboUnion = 0;
  for(int i = 0; i < subsetSize; ++i) {
    comboUnion |= context->candidateSubset[i];
  }

  for(int i = 0; i < subsetSize; ++i) {
    int cellIndex = context->hiddenComboCellIndices[i];
    int cellLocation = getCellIndexFromHousePos(house, cellIndex);
    uint16_t candidatesRemoved = puzzle->candidates[cellLocation] & ~comboUnion;
    puzzle->candidates[cellLocation] &= comboUnion;
    if(candidatesRemoved) {
      Step newStep;
      newStep.candidatesRemoved = candidatesRemoved;
      newStep.value = 0;
      switch(subsetSize) {
        case 2:
        newStep.strategyUsed = HIDDEN_PAIRS;
        break;
        case 3:
        newStep.strategyUsed = HIDDEN_TRIPLES;
        break;
        case 4: 
        newStep.strategyUsed = HIDDEN_QUADS;
      }
      
      switch (house->type) {
        case ROW:
        newStep.rowIndex = house->index;
        newStep.colIndex = cellIndex;
        break;
        case COL: 
        newStep.rowIndex = cellIndex;
        newStep.colIndex = house->index;
        break;
        default: // BLOCK
        newStep.rowIndex = cellLocation / PUZZLE_WIDTH;
        newStep.colIndex = cellLocation % PUZZLE_WIDTH;
        break;
      }
      current = appendStep(current, newStep);
    }
  }
  return current;

}
bool findHiddenCombo(HiddenComboSearchContext* context, int startIndex, int subsetSize, int depth) {
  // Check candidate subset once recursion depth is the target subsetSize. 
  if(depth == subsetSize) {
    // Get the union of the particular subset of candidates.
    uint16_t potentialComboUnion = 0;
    for(int i = 0; i < subsetSize; ++i) {
      potentialComboUnion |= context->candidateSubset[i];
    }

    int cellsWithCandidates[9];
    int cellsWithCandidatesCount = 0;
    for(int i = 0; i < context->emptyCellCount; ++i) {
      int cellIndex = context->emptyCellIndicies[i];
      uint16_t candidates = context->candidates[cellIndex];
      uint16_t hasCandidate = candidates & potentialComboUnion;
      if(hasCandidate) {
        cellsWithCandidates[cellsWithCandidatesCount] = cellIndex;
        cellsWithCandidatesCount++;
      }
    }
    if(cellsWithCandidatesCount != subsetSize) {
      return false;
    }
    bool appearsElsewhere = false;
    for(int i = 0; i < context->emptyCellCount; ++i) {
      int emptyCellIndex = context->emptyCellIndicies[i];
      bool atPotentialComboIdx = false;
      for(int k = 0; k < cellsWithCandidatesCount; ++k) {
        if(cellsWithCandidates[k] == emptyCellIndex) {
          atPotentialComboIdx = true;
          break;
        }
      }
      if(atPotentialComboIdx) {
        continue;
      }
      appearsElsewhere = context->candidates[emptyCellIndex] & potentialComboUnion;
      if(appearsElsewhere) {
        return false;
      }
    }
    // if we have searched all empty cells and only subsetSize number of cells with the candidateCombo have those candidates, we found a hidden subset.
    memcpy(context->hiddenComboCellIndices, cellsWithCandidates, sizeof(cellsWithCandidates));
    return true;
  }
  for(int i = startIndex; i < context->allCandidateCount; ++i) {
    addCandidate(&context->candidateSubset[depth], context->allCandidateArray[i]);   
    
    bool combinationFound = findHiddenCombo(context, startIndex + 1, subsetSize, depth + 1);

    if(combinationFound) {
      return true;
    }
  }
  return false;
}

StepNode* removePointingRow(int rowIndex, int skipBlockCol, uint16_t valuesToRemove, Puzzle* puzzle, StepNode* head) {
  int rowStart = rowIndex * PUZZLE_WIDTH;
  StepNode* lastStep = head;
  int candidatesToRemove[9] = {0};
  int candidateCount = getCandidatesInCell(valuesToRemove, candidatesToRemove);
  for(int i = 0; i < candidateCount; ++i) {
    uint16_t valueToRemove = 1 << (candidatesToRemove[i] -1);
    for(int colIndex = 0; colIndex < PUZZLE_WIDTH; ++colIndex) {
      if(colIndex >= skipBlockCol && colIndex < skipBlockCol + BLOCK_WIDTH) continue;
      // check what candidates will be removed
      uint16_t removed = puzzle->candidates[rowStart + colIndex] & valueToRemove;
      // remove them;
      puzzle->candidates[rowStart + colIndex] &= ~valueToRemove;
      if(removed) {
        Step newStep;
        newStep.colIndex = colIndex;
        newStep.rowIndex = rowIndex;
        newStep.candidatesRemoved = removed;
        newStep.strategyUsed = LOCKED_CANDIDATE_POINTING;
        newStep.value = 0;
        lastStep = appendStep(lastStep, newStep);
      }
    }
  }
  return lastStep;
}
StepNode* removePointingCol(int colIndex, int skipBlockRow, uint16_t valuesToRemove, Puzzle* puzzle, StepNode* head) {
  StepNode* lastStep = head;
  int candidatesToRemove[9] = {0};
  int candidateCount = getCandidatesInCell(valuesToRemove, candidatesToRemove);
  for(int i = 0; i < candidateCount; ++i) {
    uint16_t valueToRemove = 1 << (candidatesToRemove[i] - 1);
    for(int rowIndex = 0; rowIndex < PUZZLE_WIDTH; ++rowIndex) {
      if(rowIndex >= skipBlockRow && rowIndex < skipBlockRow + BLOCK_WIDTH) continue;
      uint16_t removed = puzzle->candidates[rowIndex * PUZZLE_WIDTH + colIndex] & valueToRemove;
      puzzle->candidates[rowIndex * PUZZLE_WIDTH + colIndex] &= ~valueToRemove;
      if(removed) {
        Step newStep;
        newStep.candidatesRemoved = removed;
        newStep.rowIndex = rowIndex;
        newStep.colIndex = colIndex;
        newStep.strategyUsed = LOCKED_CANDIDATE_POINTING;
        newStep.value = 0;
        lastStep = appendStep(lastStep, newStep);
      }
    }
  }
    return lastStep;
  }

bool isFullHouse(int cellIndex, int* cells) {
  int row[9] = {0};
  int rowIndex = cellIndex / PUZZLE_WIDTH;
  int colIndex = cellIndex % PUZZLE_WIDTH;
  getRow(rowIndex, cells, row);
  int emptyCount = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(row[i] == 0) {
      ++emptyCount;
      if(emptyCount > 1) break;
    }
  }
  if(emptyCount == 1) return true;

  emptyCount = 0;
  int col[9] = {0};
  getCol(colIndex, cells, col);
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(col[i] == 0) {
      ++emptyCount;
      if(emptyCount > 1) break;
    }
  }
  if(emptyCount == 1) return true;
  
  emptyCount = 0;
  int block[9] = {0};
  int blockX = colIndex / BLOCK_WIDTH;
  int blockY = rowIndex / BLOCK_WIDTH;
  getBlock(blockX, blockY, cells, block);
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(block[i] == 0) {
      ++emptyCount;
      if(emptyCount > 1) break;
    }
  }
  if(emptyCount == 1) return true;
  return false;
}

StepNode* removeCandidateFromRow(int rowIndex, int value, Puzzle* puzzle, StepNode* head) {
  int startIndex = rowIndex * PUZZLE_WIDTH;
  StepNode* lastStep = head;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int cellIndex = i + startIndex;
    if(puzzle->cells[cellIndex] != 0) continue;
    bool removed = removeCandidate(&puzzle->candidates[cellIndex], value);
    if(removed) {
      Step newStep;
      newStep.candidatesRemoved = (1 << (value - 1));
      newStep.colIndex = cellIndex % PUZZLE_WIDTH;
      newStep.rowIndex = rowIndex;
      newStep.strategyUsed = NONE;
      newStep.value = 0;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}
StepNode* removeCandidateFromCol(int colIndex, int value, Puzzle* puzzle, StepNode* head) {
  StepNode* lastStep = head;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int cellIndex = i * PUZZLE_WIDTH + colIndex;
    if(puzzle->cells[cellIndex] != 0) continue;
    bool removed = removeCandidate(&puzzle->candidates[cellIndex], value);
    if(removed) {
      Step newStep;
      newStep.candidatesRemoved = 1 << (value - 1);
      newStep.colIndex = colIndex;
      newStep.rowIndex = i;
      newStep.strategyUsed = NONE;
      newStep.value = 0;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}

bool makeGuess(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  int cellIndex = findEmptyCell(puzzle);
  if(cellIndex == -1 && isPuzzleSolved(puzzle->cells)) {
    return current;
  }
  int candidateArray[9] = {0};
  int candidateCount = getCandidatesInCell(puzzle->candidates[cellIndex], candidateArray);
  for(int i = 0; i < candidateCount; ++i) {
    puzzle->cells[cellIndex] = candidateArray[i];
    int rowIndex = cellIndex / PUZZLE_WIDTH;
    int colIndex = cellIndex % PUZZLE_WIDTH;
    Step guess = {
        .rowIndex = rowIndex,
        .colIndex = colIndex,
        .strategyUsed = GUESS,
        .value = candidateArray[i],
        .candidatesRemoved = 0
      };
    StepNode* lastStepBeforeGuess = current;
    current = appendStep(current, guess);
    current = removeCandidateFromRow(rowIndex, candidateArray[i], puzzle, current);
    current = removeCandidateFromCol(colIndex, candidateArray[i], puzzle, current);
    BlockCoord coords = {
      .blockX = colIndex / BLOCK_WIDTH,
      .blockY = rowIndex / BLOCK_WIDTH
    };
    current = removeCandidateFromBlock(coords, candidateArray[i], -1,-1, NONE, puzzle, current);

    // Recursively attempt to solve puzzle after making a guess.
    if(solveRecursive(puzzle, current)) {
      return true;
    }

    while(current != lastStepBeforeGuess) {
      int cellIndex = current->step.rowIndex * PUZZLE_WIDTH + current->step.colIndex;

      if(current->step.candidatesRemoved) {
        puzzle->candidates[cellIndex] |= current->step.candidatesRemoved;
      } else {
        puzzle->cells[cellIndex] = 0;
      }
      current = current->prev;
    }
    freeStepList(current->next); // free memory for the steps that didn't lead to a solution.
  }
  return false;

}
StepNode* applyFoundValue(Puzzle* puzzle, int value, int cellIndex, Strategy stratUsed, StepNode* head) {
  StepNode* current = head;
  puzzle->cells[cellIndex] = value;
  puzzle->candidates[cellIndex] = 0;

  int rowIndex = cellIndex / PUZZLE_WIDTH;
  int colIndex = cellIndex % PUZZLE_WIDTH;
  Step newStep = {
    .value = value,
    .rowIndex = rowIndex,
    .colIndex = colIndex,
    .candidatesRemoved = false,
    .strategyUsed = stratUsed    
  };
  current = appendStep(head, newStep);
  current = removeCandidateFromRow(rowIndex, value, puzzle, current);
  current = removeCandidateFromCol(colIndex, value, puzzle, current);
  current = removeCandidateFromBlock(
    (BlockCoord){.blockX = colIndex / BLOCK_WIDTH, .blockY = rowIndex / BLOCK_WIDTH},
    value, 
    -1, 
    -1, 
    NONE, 
    puzzle, 
    current
  );
  return current;
}