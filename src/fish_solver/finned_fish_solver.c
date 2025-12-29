#include "finned_fish_solver.h"
StepNode* findFinnedFish(Puzzle* puzzle, StepNode* head) {

}

StepNode* findFinnedXWing(Puzzle* puzzle, StepNode* head) {

}

StepNode* findFinnedFishByRow(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head) {
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    uint16_t candidateMask = (1 << (candidate - 1));
    uint16_t rowCandidates[9] = {0};
    uint16_t rowsWithCandidate = findRowsWithCandidate(puzzle->candidates, candidate);
    int rowCount = __builtin_popcount(rowsWithCandidate);
    
    
    FishBases potentialBase = {
      .bases = {0},
      .numOfBases = 0
    };

    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      getCandidateRow(i, puzzle->candidates, rowCandidates);

    //   int candidateCount = countCandidateInHouse(rowCandidates, candidateMask, fishSize);
    //   if(candidateCount > 0) {
    //     fishBases.numOfBases++;
    //     if(fishBases.numOfBases > fishSize) break;
    //     FishIndicies* baseHouse = &fishBases.bases[fishBases.numOfBases - 1];

    //     baseHouse->baseIndex = i;
    //     findCandidateIndiciesInHouse(rowCandidates, candidateMask, &baseHouse->indicies, candidateCount);
    //   }
    }
    // if(fishBases.numOfBases != fishSize) continue;

  }
}

bool searchFishRecursively(FinnedFishContext* context, int startIndex, int depth, int fishSize) {
  if(depth == fishSize) {
    uint16_t colsWithCandidates = findColsWithCandidate(context->allCandidates, context->candidate);
    
    // uint16_t colCandidates[9] = {0};
    // for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    //   getCandidateCol(i, context->allCandidates, colCandidates);

    // }
  }

  for(int i = startIndex; i < context->houseWithCandidateCount; ++i) {
    context->potenialFishBases->bases[depth].baseIndex = context->indiciesWithCandidate[i];
    if(searchFishRecursively(context, startIndex + 1, depth + 1, fishSize)) {
      return true;
    }
    
  }
}

uint16_t findRowsWithCandidate(uint16_t* puzzleCandidates, int candidate) {
  uint16_t rowsWithCandidate = 0;
  uint16_t candidateMask = 1 << (candidate - 1);
  uint16_t candidateRow[9] = {0};
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    getCandidateRow(i, puzzleCandidates, candidateRow);
    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      if(hasCandidate(candidateRow[k], candidate)) {
        rowsWithCandidate |= candidateMask;
        break;
      }
    }
  }
  return rowsWithCandidate;
}

uint16_t findColsWithCandidate(uint16_t* puzzleCandidates, int candidate) {
  uint16_t rowsWithCandidate = 0;
  uint16_t candidateMask = 1 << (candidate - 1);
  uint16_t candidateCol[9] = {0};
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    getCandidateCol(i, puzzleCandidates, candidateCol);
    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      if(hasCandidate(candidateCol[k], candidate)) {
        rowsWithCandidate |= candidateMask;
        break;
      }
    }
  }
  return rowsWithCandidate;
}
