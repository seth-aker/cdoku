#include "puzzle.h"

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

int getCellIndexFromHousePos(House* house, int cellIndex) {
  switch (house->type) {
  case BLOCK:
    int blockXOffset = (house->index % BLOCK_WIDTH) * BLOCK_WIDTH;
    int blockYOffset = (house->index / BLOCK_WIDTH) * PUZZLE_WIDTH * 3; // 3 Rows of 9 cells
    int blockStart = blockXOffset + blockYOffset;

    int rowNum = cellIndex / BLOCK_WIDTH;
    int colNum = cellIndex % BLOCK_WIDTH;

    return blockStart + colNum + (rowNum * PUZZLE_WIDTH);
    break;
  case COL:
    return house->index + (cellIndex * PUZZLE_WIDTH);
    break;
  
  default: // ROW
    return cellIndex + (house->index * PUZZLE_WIDTH);
    break;
  }
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

int findEmptyCell(Puzzle* puzzle) {
  for(int i = 0; i < PUZZLE_WIDTH * PUZZLE_WIDTH; ++i) {
    if(puzzle->cells[i] == 0) {
      return i;
    }
  }
  return -1;
}

bool isLocationInBlock(int row, int col, int blockIndex) {
  int blockX = col / BLOCK_WIDTH;
  int blockY = row / BLOCK_WIDTH;

  return blockX + blockY * BLOCK_WIDTH == blockIndex;
}
StepNode* removeCandidateFromBlock(BlockCoord blockCoords, int value, int skipRow, int skipCol, Strategy stratUsed, Puzzle* puzzle, StepNode* head) {
  StepNode* lastStep = head;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(skipRow != -1 && (i / BLOCK_WIDTH) == skipRow) continue;
    if(skipCol != -1 && (i % BLOCK_WIDTH) == skipCol) continue;
    int rowModifier = (i / BLOCK_WIDTH) * PUZZLE_WIDTH;
    int colModifier = i % BLOCK_WIDTH;
    int blockXOffset = blockCoords.blockX * BLOCK_WIDTH;
    int blockYOffset = blockCoords.blockY * BLOCK_WIDTH * PUZZLE_WIDTH;
    int cellIndex = rowModifier + colModifier + blockXOffset + blockYOffset;

    bool removed = removeCandidate(&puzzle->candidates[cellIndex], value);
    if(removed) {
      Step newStep;
      newStep.candidatesRemoved = 1 << (value - 1);
      newStep.colIndex = cellIndex % PUZZLE_WIDTH;
      newStep.rowIndex = cellIndex / PUZZLE_WIDTH;
      newStep.strategyUsed = stratUsed;
      newStep.value = value;
      lastStep = appendStep(lastStep, newStep);
    }
  }
  return lastStep;
}