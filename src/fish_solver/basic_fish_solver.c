#include "basic_fish_solver.h"

StepNode* findBasicFish(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findBasicXWing(puzzle, current);
  if(current != head) {
    return current;
  }
  current = findBasicSwordfish(puzzle, current);
  if(current != head) {
    return current;
  }
  current = findBasicJellyfish(puzzle, current);
  return current;
}

StepNode* findBasicXWing(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findBasicFishByRow(puzzle, 2, current);
  if(current != head) {
    return current;
  }
  current = findBasicFishByCol(puzzle, 2, current);
  return current;
}

StepNode* findBasicSwordfish(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findBasicFishByRow(puzzle, 3, current);
  if(current != head) {
    return current;
  }
  current = findBasicFishByCol(puzzle, 3, current);
  return current;
}
StepNode* findBasicJellyfish(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findBasicFishByRow(puzzle, 4, current);
  if(current != head) {
    return current;
  }
  current = findBasicFishByCol(puzzle, 4, current);
  return current;
}

StepNode* findBasicFishByRow(Puzzle* puzzle, int fishSize, StepNode* head) {
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    uint16_t candidateMask = (1 << (candidate - 1));
    uint16_t rowCandidates[9] = {0};
    FishBases fishBases = {
      .numOfBases = 0
    };
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      getCandidateRow(i, puzzle->candidates, rowCandidates);
      int candidateCount = countCandidateInHouse(rowCandidates, candidateMask, fishSize);
      
      if(candidateCount > 0 && candidateCount <= fishSize) {
        fishBases.numOfBases++;
        if(fishBases.numOfBases > fishSize) break;
        FishIndicies* baseHouse = fishBases.bases[fishBases.numOfBases - 1];
        baseHouse->baseIndex = i;
        findCandidateIndiciesInHouse(rowCandidates, candidateMask, &baseHouse->indicies, candidateCount);
      }
    }
    if(fishBases.numOfBases != fishSize) continue;

    uint16_t candidateIndexUnion = 0;    
    int exceptRows[fishSize];
    for(int i = 0; i < fishBases.numOfBases; ++i) {
      candidateIndexUnion |= fishBases.bases[i]->indicies;
      exceptRows[i] = fishBases.bases[i]->baseIndex;
    }
    int indicies[fishSize];
    int count = getCandidatesInCell(candidateIndexUnion, indicies); // This isn't actually candidates in a cell, this is indicies of where candidates exist in a row.
    if(count != fishSize) continue;

    StepNode* current = head;
    for(int i = 0; i < fishBases.numOfBases; ++i) {
      current = removeCandidatesFromColExcept(puzzle, candidateMask, indicies[i], exceptRows, fishSize, head);
    }
    if(current != head) {
      return current;
    }
  }
  return head;
}

StepNode* findBasicFishByCol(Puzzle* puzzle, int fishSize, StepNode* head) {
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    uint16_t candidateMask = 1 << (candidate - 1);

    uint16_t colCandidates[9] = {0};
    FishBases fishBases = {
      .numOfBases = 0
    };
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      getCandidateCol(i, puzzle->candidates, colCandidates);
      int candidateCount = countCandidateInHouse(colCandidates, candidateMask, fishSize);
      
      if(candidateCount > 0 && candidateCount <= fishSize) {
        fishBases.numOfBases++;
        if(fishBases.numOfBases > fishSize) break;
        FishIndicies* baseHouse = fishBases.bases[fishBases.numOfBases - 1];
        baseHouse->baseIndex = i;
        findCandidateIndiciesInHouse(colCandidates, candidateMask, &baseHouse->indicies, candidateCount);
      }
    }
    if(fishBases.numOfBases != fishSize) continue;

    uint16_t candidateIndexUnion = 0;    
    int exceptRows[fishSize];
    for(int i = 0; i < fishBases.numOfBases; ++i) {
      candidateIndexUnion |= fishBases.bases[i]->indicies;
      exceptRows[i] = fishBases.bases[i]->baseIndex;
    }
    int indicies[fishSize];
    int count = getCandidatesInCell(candidateIndexUnion, indicies); // This isn't actually candidates in a cell, this is indicies of where candidates exist in a row.
    
    if(count != fishSize) continue;

    StepNode* current = head;
    for(int i = 0; i < fishBases.numOfBases; ++i) {
      current = removeCandidatesFromRowExcept(puzzle, candidateMask, indicies[i], exceptRows, fishSize, head);
    }
    if(current != head) {
      return current;
    }
  }
  return head;
}

void findCandidateIndiciesInHouse(uint16_t* house, uint16_t candidateMask, uint16_t* indicies, int indiciesSize) {
  int count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house[i] & candidateMask) {
      *indicies |= candidateMask;
      ++count;
    }
    if(count == indiciesSize) return;
  }
}

// Count how many times a candidate exists in a row. Max Count is used to short circuit the search, pass 9 if you want to always search the entire row.
int countCandidateInHouse(uint16_t* house, uint16_t candidateMask, int maxCount) {
  int count = 0;
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(house[i] & candidateMask) {
      ++count;
    }
    if(count > maxCount) {
      return count;
    }
  }
  return count;
} 

StepNode* removeCandidatesFromColExcept(Puzzle* puzzle, uint16_t candidateMask, int colIndex, int* exceptIndicies, int exceptSize, StepNode* head) {
  StepNode* current = head;
  for(int rowIndex = 0; rowIndex < PUZZLE_WIDTH; ++rowIndex) {
    if(includes(exceptIndicies, exceptSize, rowIndex)) continue;
    int cellIndex = rowIndex * PUZZLE_WIDTH + colIndex;
    uint16_t cellHasCandidate = puzzle->candidates[cellIndex] & candidateMask;
    if(cellHasCandidate) {
      puzzle->candidates[cellIndex] &= ~candidateMask;
      Step newStep = {
        .candidatesRemoved = candidateMask,
        .colIndex = colIndex,
        .rowIndex = rowIndex,
        .strategyUsed = X_WING,
        .value = 0        
      };
      current = appendStep(current, newStep);
    }
  }
  return current;
}

StepNode* removeCandidatesFromRowExcept(Puzzle* puzzle, uint16_t candidateMask, int rowIndex, int* exceptIndicies, int exceptSize, StepNode* head) {
  StepNode* current = head;
  for(int colIndex = 0; colIndex < PUZZLE_WIDTH; ++colIndex) {
    if(includes(exceptIndicies, exceptSize, colIndex)) continue;
    int cellIndex = rowIndex * PUZZLE_WIDTH + colIndex;
    uint16_t cellHasCandiate = puzzle->candidates[cellIndex] & candidateMask;
    if(cellHasCandiate) {
      puzzle->candidates[cellIndex] &= ~candidateMask;
      Step newStep = {
        .candidatesRemoved = candidateMask,
        .colIndex = colIndex,
        .rowIndex = rowIndex,
        .strategyUsed = X_WING,
        .value = 0
      };
      current = appendStep(current, newStep);
    }
  }
  return current;
}