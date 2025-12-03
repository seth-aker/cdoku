#include "sudoku_solver.h"

bool solvePuzzle(Puzzle* puzzle, StepNode* head) {
  if(head == NULL) {
    return false;
  }
  StepNode* current = head;
  fillPuzzleCandidates(puzzle);
  while(!isPuzzleSolved(puzzle->cells)) {
    // look for a single and if one exists adds it and updates current.
    // Find single returns the updated node if a single was found OR the same node that was passed.
    StepNode* newStep = findSingle(puzzle, current);
    if(current != newStep) {
      current = newStep;
      int cellIndex = current->step.rowIndex * PUZZLE_WIDTH + current->step.colIndex;
      puzzle->cells[cellIndex] = current->step.value;
      puzzle->candidates[cellIndex] = 0;
      current = removeCandidateFromRow(newStep->step.rowIndex, newStep->step.value, puzzle, current);
      current = removeCandidateFromCol(newStep->step.colIndex, newStep->step.value, puzzle, current);
      current = removeCandidateFromBlock(newStep->step.colIndex / BLOCK_WIDTH, newStep->step.rowIndex / BLOCK_WIDTH, newStep->step.value, -1, -1, puzzle, current);
      continue;
    }
    newStep = findLockedCandidates(puzzle, current);
    if(current != newStep) {
      // Puzzle has been modified and we need to check if it can be solved/go through the loop again.
      continue;
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

void fillPuzzleCandidates(Puzzle* puzzle) {
  for(int potentialCandidate = 1; potentialCandidate <= PUZZLE_WIDTH; ++potentialCandidate) {
    for(int row = 0; row < PUZZLE_WIDTH; ++row) {
      for(int col = 0; col < PUZZLE_WIDTH; ++col) {
        int cellIndex = row * PUZZLE_WIDTH + col;
        int cellVal = puzzle->cells[cellIndex];
        if(cellVal != 0 || !numWorksInCell(row, col, potentialCandidate, puzzle->cells)) {
          continue;
        } else {
          addCandidate(&puzzle->candidates[cellIndex], potentialCandidate);
        }
      }
    }
  }
}



StepNode* findSingle(Puzzle* puzzle, StepNode* head) {
  Step newStep;
  for(int rowIndex = 0; rowIndex < PUZZLE_WIDTH; ++rowIndex) {
    for(int colIndex = 0; colIndex < PUZZLE_WIDTH; ++colIndex) {
      int cellIndex = rowIndex * PUZZLE_WIDTH + colIndex;
      if(puzzle->cells[cellIndex] != 0) {
        continue;
      }
      int count = __builtin_popcount(puzzle->candidates[cellIndex]);
      if(count == 1) {
        // TODO: Optimize this to short circut
        if(isFullHouse(rowIndex, colIndex, puzzle->cells)) {
          newStep.strategyUsed = FULL_HOUSE;
        } else {
          newStep.strategyUsed = NAKED_SINGLE;
        }
        newStep.rowIndex = rowIndex;
        newStep.colIndex = colIndex;
        newStep.candidateRemoved = false;
        newStep.value = getValueFromMask(puzzle->candidates[cellIndex]);
        StepNode* last = appendStep(head, newStep);
        return last;
      }
      int hiddenSingle = findHiddenSingle(rowIndex, colIndex, puzzle);
      if(hiddenSingle) {
        newStep.rowIndex = rowIndex;
        newStep.colIndex = colIndex;
        newStep.candidateRemoved = false;
        newStep.strategyUsed = HIDDEN_SINGLE;
        newStep.value = hiddenSingle;
        StepNode* last = appendStep(head, newStep);
        return last;
      }
    }
  }
  return head;
}
int findHiddenSingle(int rowIndex, int colIndex, Puzzle* puzzle) {
  int rowStart = rowIndex * PUZZLE_WIDTH;
  int cellIndex = rowStart + colIndex;
  if(puzzle->cells[cellIndex] != 0) {
    return -1;
  }
  uint16_t cell = puzzle->candidates[cellIndex];
  
  uint16_t others = 0;
  // Check the row;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i == colIndex) continue;
    others |= puzzle->candidates[rowStart + i];
  }
  uint16_t unique = cell & ~others;
  if(unique) return getValueFromMask(unique);

  others = 0;

  // Check the col;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i == rowIndex) continue;
    others |= puzzle->candidates[i * PUZZLE_WIDTH + colIndex];
  }
  unique = cell & ~others;
  if(unique) return getValueFromMask(unique);

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
  if(unique) return getValueFromMask(unique);
  return 0;
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
        int candidatesToRemove[9] = {0};
        int candidateCount = getCandidatesInCell(uRow1, candidatesToRemove);
        for(int i = 0; i < candidateCount; ++i) {
          StepNode* lastChange = removePointingRow(blockY * BLOCK_WIDTH, blockX * BLOCK_WIDTH, candidatesToRemove[i], puzzle, head);
          if(lastChange != head) {
            return lastChange;
          }
        }
      }
      if(uRow2) {
        int candidatesToRemove[9] = {0};
        int candidateCount = getCandidatesInCell(uRow2, candidatesToRemove);
        for(int i = 0; i < candidateCount; ++i) {
          StepNode* lastChange = removePointingRow(blockY * BLOCK_WIDTH + 1, blockX * BLOCK_WIDTH, candidatesToRemove[i], puzzle, head);
          if(lastChange != head) {
            return lastChange;
          }
        }
      }
      if(uRow3) {
        int candidatesToRemove[9] = {0};
        int candidateCount = getCandidatesInCell(uRow3, candidatesToRemove);
        for(int i = 0; i < candidateCount; ++i) {
          StepNode* lastChange = removePointingRow(blockY * BLOCK_WIDTH + 2, blockX * BLOCK_WIDTH, candidatesToRemove[i], puzzle, head);
          if(lastChange != head) {
            return lastChange;
          }
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
        int candidatesToRemove[9] = {0};
        int candidateCount = getCandidatesInCell(uCol1, candidatesToRemove);
        for(int i = 0; i < candidateCount; ++i) {
          StepNode* lastChange = removePointingCol(blockX * BLOCK_WIDTH, blockY * BLOCK_WIDTH, candidatesToRemove[i], puzzle, head);
          if(lastChange == head) {
            return lastChange;
          }
        }
      }
      if(uCol2) {
        int candidatesToRemove[9] = {0};
        int candidateCount = getCandidatesInCell(uCol2, candidatesToRemove);
        for(int i = 0; i < candidateCount; ++i) {
          StepNode* lastChange = removePointingCol(blockX * BLOCK_WIDTH + 1, blockY * BLOCK_WIDTH, candidatesToRemove[i], puzzle, head);
          if(lastChange != head) {
            return lastChange;
          }
        }
      }
      if(uCol3) {
        int candidatesToRemove[9] = {0};
        int candidateCount = getCandidatesInCell(uCol3, candidatesToRemove);
        for(int i = 0; i < candidateCount; ++i) {
          StepNode* lastChange = removePointingCol(blockX * BLOCK_WIDTH + 2, blockY * BLOCK_WIDTH, candidatesToRemove[i], puzzle, head);
          if(lastChange != head) {
            return lastChange;
          }
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
        StepNode* lastStep = removeCandidateFromBlock(0, (rowIndex / BLOCK_WIDTH), candidatesToRemove[i], rowIndex % BLOCK_WIDTH, -1, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uRowSeg2) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uRowSeg2, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        StepNode* lastStep = removeCandidateFromBlock(1, (rowIndex / BLOCK_WIDTH), candidatesToRemove[i], rowIndex % BLOCK_WIDTH, -1, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uRowSeg3) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uRowSeg3, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        StepNode* lastStep = removeCandidateFromBlock(2, (rowIndex / BLOCK_WIDTH), candidatesToRemove[i], rowIndex % BLOCK_WIDTH, -1, puzzle, head);
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
        StepNode* lastStep = removeCandidateFromBlock(colIndex / BLOCK_WIDTH, 0, candidatesToRemove[i], -1, colIndex % BLOCK_WIDTH, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uColSeg2) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uColSeg2, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        StepNode* lastStep = removeCandidateFromBlock(colIndex / BLOCK_WIDTH, 0, candidatesToRemove[i], -1, colIndex % BLOCK_WIDTH, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
    if(uColSeg3) {
      int candidatesToRemove[9] = {0};
      int candidateCount = getCandidatesInCell(uColSeg3, candidatesToRemove);
      for(int i = 0; i < candidateCount; ++i) {
        StepNode* lastStep = removeCandidateFromBlock(colIndex / BLOCK_WIDTH, 0, candidatesToRemove[i], -1, colIndex % BLOCK_WIDTH, puzzle, head);
        if(lastStep != head) {
          return lastStep;
        }
      }
    }
  }
  return head;
}

StepNode* findSubsets(Puzzle* puzzle, StepNode* head) {

}

StepNode* findNakedSubsetOfSize(Puzzle* puzzle, int size, StepNode* head) {

}

StepNode* findNakedSubsetInHouse(int* houseValues, int* houseCandidates, int subsetSize, StepNode* head) {
  
  NakedComboSearchContext context;
  context.candidates = houseCandidates;
  context.values = houseValues;
  context.foundCount = 0;
  context.foundIndicies[subsetSize];
  context.indicies[4]; // max size of subsetSize;


// TODO: modify this to account for the unique combos right now it is not doing that.
  // uint16_t candidateUnions[numOfCombos];
  // for(int i = 0; i < numOfCombos; ++i) {
  //   for(int j = 0; j < emptyCellCount; ++j) {
  //     candidateUnions[i] |= houseCandidates[emptyCellsIndicies[j]];
  //   }
  //   int candidates[9];
  //   if(getCandidatesInCell(candidateUnions[i], candidates) == subsetSize) {
  //     for(int houseIndex = 0; houseIndex < PUZZLE_WIDTH; ++houseIndex) {
  //       if(includes(emptyCellsIndicies, emptyCellCount, houseIndex)) {
  //         continue;
  //       }
  //       for(int unionIndex = 0; unionIndex < numOfCombos; ++unionIndex) {
          
  //       }
  //     }
  //   }
  // }

  
}

StepNode* removePointingRow(int rowIndex, int skipBlockCol, int valueToRemove, Puzzle* puzzle, StepNode* head) {
  int rowStart = rowIndex * PUZZLE_WIDTH;
  StepNode* lastStep = head;
  for(int colIndex = 0; colIndex < PUZZLE_WIDTH; ++colIndex) {
    if(colIndex >= skipBlockCol && colIndex < skipBlockCol + BLOCK_WIDTH) continue;
    bool removed = removeCandidate(&puzzle->candidates[rowStart + colIndex], valueToRemove);
    if(removed) {
      Step newStep;
      newStep.colIndex = colIndex;
      newStep.rowIndex = rowIndex;
      newStep.candidateRemoved = true;
      newStep.strategyUsed = LOCKED_CANDIDATE_POINTING;
      newStep.value = valueToRemove;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}
StepNode* removePointingCol(int colIndex, int skipBlockRow, int valueToRemove, Puzzle* puzzle, StepNode* head) {
  StepNode* lastStep = head;
  for(int rowIndex = 0; rowIndex < PUZZLE_WIDTH; ++rowIndex) {
    if(rowIndex >= skipBlockRow && rowIndex < skipBlockRow + BLOCK_WIDTH) continue;
    bool removed = removeCandidate(&puzzle->candidates[rowIndex * PUZZLE_WIDTH + colIndex], valueToRemove);
    if(removed) {
      Step newStep;
      newStep.candidateRemoved = true;
      newStep.rowIndex = rowIndex;
      newStep.colIndex = colIndex;
      newStep.strategyUsed = LOCKED_CANDIDATE_POINTING;
      newStep.value = valueToRemove;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}


bool numWorksInCell(int rowIndex, int colIndex, int potentialNum, int* cells) {
  int block[9] = {0};
  int row[9] = {0};
  int col[9] = {0};
  int blockY = rowIndex / BLOCK_WIDTH;
  int blockX = colIndex / BLOCK_WIDTH;
  getBlock(blockX, blockY, cells, block);
  getRow(rowIndex, cells, row);
  getCol(colIndex, cells, col);

  int cellPosInBlock = getCellPosInBlock(rowIndex, colIndex);
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i != cellPosInBlock && block[i] == potentialNum) {
      return false;
    }
    if(i != colIndex && row[i] == potentialNum) {
      return false;
    }
    if(i != rowIndex && col[i] == potentialNum) {
      return false;
    }
  }
  return true;
}

void getBlock(int blockX, int blockY, int* cells, int* block) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int rowModifier = (i / BLOCK_WIDTH) * PUZZLE_WIDTH;
    int colModifier = i % BLOCK_WIDTH;
    int blockXOffset = blockX * BLOCK_WIDTH;
    int blockYOffset = blockY * BLOCK_WIDTH * PUZZLE_WIDTH;
    int cellIndex = rowModifier + colModifier + blockXOffset + blockYOffset;
    
    block[i] = cells[cellIndex];
  }
}

void getRow(int rowIndex, int* cells, int* row) {
  int offset = rowIndex * PUZZLE_WIDTH;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    row[i] = cells[i + offset];
  }
}

void getCol(int colIndex, int* cells, int* col) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    col[i] = cells[i * PUZZLE_WIDTH + colIndex];
  }
}

int getCellPosInBlock(int rowIndex, int colIndex) {
  int blockX = colIndex / BLOCK_WIDTH;
  int blockY = rowIndex / BLOCK_WIDTH;
  int normalizedX = colIndex - (blockX * BLOCK_WIDTH);
  int normalizedY = rowIndex - (blockY * BLOCK_WIDTH);

  return (normalizedY * BLOCK_WIDTH) + normalizedX;
}

void getCandidateBlock(int blockX, int blockY, uint16_t* candidates, uint16_t* block) {
  int blockXOffset = blockX * BLOCK_WIDTH;
  int blockYOffset = blockY * BLOCK_WIDTH * PUZZLE_WIDTH;
  int startIndex = blockXOffset + blockYOffset;
  // TODO: Unwrap this function for speed;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int rowModifier = (i / BLOCK_WIDTH) * PUZZLE_WIDTH;
    int colModifier = i % BLOCK_WIDTH;
    int cellIndex = startIndex + rowModifier + colModifier;
    block[i] = candidates[cellIndex];
  }
}

void getCandidateRow(int rowIndex, uint16_t* candidates, uint16_t* row) {
  int offset = rowIndex * PUZZLE_WIDTH;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    row[i] = candidates[i + offset];
  }
}
void getCandidateCol(int colIndex, uint16_t* candidates, uint16_t* col) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int offset = i * PUZZLE_WIDTH + colIndex;
    col[i] = candidates[offset];
  }
}

bool isFullHouse(int rowIndex, int colIndex, int* cells) {
  int row[9] = {0};
  getRow(rowIndex, cells, row);
  if(countFilledCells(row) == 8) {
    return true;
  }

  int col[9] = {0};
  getCol(colIndex, cells, col);
  if(countFilledCells(col) == 8) {
    return true;
  }

  int block[9] = {0};
  int blockX = colIndex / BLOCK_WIDTH;
  int blockY = rowIndex / BLOCK_WIDTH;
  getBlock(blockX, blockY, cells, block);
  if(countFilledCells(block) == 8) {
    return true;
  }
  return false;
}

int getCandidatesInCell(uint16_t cellCandidates, int* candidatesArray) {
  int count = 0;
  while(cellCandidates > 0) {
    int candidateIndex = __builtin_ctz(cellCandidates);
    candidatesArray[count++] = candidateIndex + 1;
    // Removes the lowest set bit.
    cellCandidates &= (cellCandidates - 1);
  }
  return count;
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
      newStep.candidateRemoved = true;
      newStep.colIndex = cellIndex % PUZZLE_WIDTH;
      newStep.rowIndex = rowIndex;
      newStep.strategyUsed = NONE;
      newStep.value = value;
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
      newStep.candidateRemoved = true;
      newStep.colIndex = colIndex;
      newStep.rowIndex = i;
      newStep.strategyUsed = NONE;
      newStep.value = value;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}

StepNode* removeCandidateFromBlock(int blockX, int blockY, int value, int skipRow, int skipCol, Puzzle* puzzle, StepNode* head) {
  StepNode* lastStep = head;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(skipRow != -1 && (i / BLOCK_WIDTH) == skipRow) continue;
    if(skipCol != -1 && (i % BLOCK_WIDTH) == skipCol) continue;
    int rowModifier = (i / BLOCK_WIDTH) * PUZZLE_WIDTH;
    int colModifier = i % BLOCK_WIDTH;
    int blockXOffset = blockX * BLOCK_WIDTH;
    int blockYOffset = blockY * BLOCK_WIDTH * PUZZLE_WIDTH;
    int cellIndex = rowModifier + colModifier + blockXOffset + blockYOffset;

    bool removed = removeCandidate(&puzzle->candidates[cellIndex], value);
    if(removed) {
      Step newStep;
      newStep.candidateRemoved = true;
      newStep.colIndex = (blockYOffset + colModifier) % PUZZLE_WIDTH;
      newStep.rowIndex = (blockXOffset + rowModifier);
      newStep.strategyUsed = NONE;
      newStep.value = value;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}
void checkCombination(NakedComboSearchContext* context, int startIndex, int subsetSize, int depth) {
  if(depth == subsetSize) {
    uint16_t unionMask = 0;
    for(int i = 0; i < subsetSize; ++i) {
      int cellIndex = context->indicies[i];
      unionMask |= context->candidates[cellIndex];
    }
    if(__builtin_popcount(unionMask) == subsetSize) {
      for(int i = 0; i < PUZZLE_WIDTH; ++i) {
        for(int j = 0; j < subsetSize; ++j) {
          if(context->indicies[j] == i) {
            context->foundIndicies[context->foundCount] = i;
            context->foundCount++;
          }
        }
      }
    }
    return;
  }
  for(int i = startIndex; i < PUZZLE_WIDTH; ++i) {
    if(context->values[i] > 0) continue;

    if(__builtin_popcount(context->candidates[i]) > subsetSize) continue;

    context->indicies[depth] = i;
    checkCombination(context, startIndex + 1, subsetSize, depth + 1);
  }
}
