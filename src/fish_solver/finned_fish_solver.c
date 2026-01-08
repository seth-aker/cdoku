#include "finned_fish_solver.h"
#include "utils.h"
StepNode* findFinnedFish(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findFinnedXWing(puzzle, current);
  if(current != head) {
    return current;
  }
  current = findFinnedSwordfish(puzzle, current);
  if(current != head) {
    return current;
  }
  current = findFinnedJellyfish(puzzle, current);
  return current;
}

StepNode* findFinnedXWing(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findFinnedFishByRow(puzzle, 2, FINNED_X_WING, current);
  current = findFinnedFishByCol(puzzle, 2, FINNED_X_WING, current);
  return current;
}
StepNode* findFinnedSwordfish(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findFinnedFishByRow(puzzle, 3, FINNED_SWORDFISH, current);
  current = findFinnedFishByCol(puzzle, 3, FINNED_SWORDFISH, current);
  return current;
}

StepNode* findFinnedJellyfish(Puzzle* puzzle, StepNode* head) {
  StepNode* current = head;
  current = findFinnedFishByRow(puzzle, 4, FINNED_JELLYFISH, current);
  current = findFinnedFishByCol(puzzle, 4, FINNED_JELLYFISH, current);
  return current;
}
StepNode* findFinnedFishByRow(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head) {
  StepNode* current = head;
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    uint16_t candidateMask = (1 << (candidate - 1));
    uint16_t rowsWithCandidate = findRowsWithCandidate(puzzle->candidates, candidate);
    int rowIndicies[9] = {0};
    int rowCount = maskToIntArray(rowsWithCandidate, rowIndicies);
    if(rowCount > fishSize) continue;

    FishBases potentialBases = {
      .bases = {0},
      .numOfBases = 0
    };
    FishBases baseCombos = {
      .bases = {0},
      .numOfBases = 0
    };
    for(int i = 0; i < rowCount; ++i) {
      potentialBases.bases[i].baseIndex = rowIndicies[i];
      uint16_t rowCandidates[9] = {0};
      getCandidateRow(rowIndicies[i], puzzle->candidates, rowCandidates);
      findCandidateIndiciesInHouse(rowCandidates, candidateMask, &potentialBases.bases[i].indicies, -1);
      potentialBases.numOfBases++;
    }  
    int indiciesToRemove[9] = {0};
    FinnedFishContext context = {
      .housesWithCandidateCount = rowCount,
      .allFishBases = &potentialBases,
      .potentialBaseCombo = &baseCombos,
      .allCandidates = &puzzle->candidates[0],
      .candidate = candidate,
      .byRow = true,
      .indiciesToRemove = indiciesToRemove,
      .indiciesToRemoveCount = 0
    };
    if(searchFishRecursively(&context, 0, 0, fishSize)) {
      for(int i = 0; i < context.indiciesToRemoveCount; ++i) {
        puzzle->candidates[context.indiciesToRemove[i]] &= ~candidateMask;
        Step newStep = {
          .candidatesRemoved = candidateMask,
          .colIndex = context.indiciesToRemove[i] % PUZZLE_WIDTH,
          .rowIndex = context.indiciesToRemove[i] / PUZZLE_WIDTH,
          .value = 0,
        };
        switch (fishSize)
        {
        case 2:
          newStep.strategyUsed = FINNED_X_WING;
          break;
        case 3:
          newStep.strategyUsed = FINNED_SWORDFISH;
          break;
        default:
          newStep.strategyUsed = FINNED_JELLYFISH;
          break;
        }
        current = appendStep(current, newStep);
        return current;
      }
    }    

  }
  return current;
}
StepNode* findFinnedFishByCol(Puzzle* puzzle, int fishSize, Strategy stratUsed, StepNode* head) {
  StepNode* current = head;
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    uint16_t candidateMask = (1 << (candidate - 1));
    uint16_t colsWithCandidate = findColsWithCandidate(puzzle->candidates, candidate);
    int colIndicies[9] = {0};
    int colCount = maskToIntArray(colsWithCandidate, colIndicies);

    if(colCount < fishSize) continue;
    FishBases potentialBases = {
      .bases = {0},
      .numOfBases = 0
    };
    FishBases baseCombos = {
      .bases = {0},
      .numOfBases = 0
    };
    for(int i = 0; i < colCount; ++i) {
      potentialBases.bases[i].baseIndex = colIndicies[i];
      uint16_t colCandidates[9] = {0};
      getCandidateRow(colIndicies[i], puzzle->candidates, colCandidates);
      findCandidateIndiciesInHouse(colCandidates, candidateMask, &potentialBases.bases[i].indicies, -1);
      potentialBases.numOfBases++;
    }
    int indiciesToRemove[9] = {0};
    FinnedFishContext context = {
      .housesWithCandidateCount = colCount,
      .allFishBases = &potentialBases,
      .potentialBaseCombo = &baseCombos,
      .allCandidates = &puzzle->candidates[0],
      .candidate = candidate,
      .byRow = false,
      .indiciesToRemove = indiciesToRemove,
      .indiciesToRemoveCount = 0
    };
    if(searchFishRecursively(&context, 0, 0, fishSize)) {
      for(int i = 0; i < context.indiciesToRemoveCount; ++i) {
        puzzle->candidates[context.indiciesToRemove[i]] &= ~candidateMask;
        Step newStep = {
          .candidatesRemoved = candidateMask,
          .colIndex = context.indiciesToRemove[i] % PUZZLE_WIDTH,
          .rowIndex = context.indiciesToRemove[i] / PUZZLE_WIDTH,
          .value = 0,
        };
        switch (fishSize)
        {
        case 2:
          newStep.strategyUsed = FINNED_X_WING;
          break;
        case 3:
          newStep.strategyUsed = FINNED_SWORDFISH;
          break;
        default:
          newStep.strategyUsed = FINNED_JELLYFISH;
          break;
        }
        current = appendStep(current, newStep);
        return current;
      }
    }
  }
  return current;
}
bool searchFishRecursively(FinnedFishContext* context, int startIndex, int depth, int fishSize) {
  if(depth == fishSize) {
    uint16_t baseCoverUnion = 0;

    for(int i = 0; i < fishSize; ++i) {
      baseCoverUnion |= context->potentialBaseCombo->bases[i].indicies; // cover indicies
      baseCoverUnion |= (1 << (context->potentialBaseCombo->bases[i].baseIndex)); // base index
    }
    if(__builtin_popcount(baseCoverUnion) <= fishSize) {
      return false; // this is a basic fish or none at all.
    }
    int blockIndicies[9] = {0};
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      int blockX = i % BLOCK_WIDTH;
      int blockY = i / BLOCK_WIDTH;
      getBlockIndicies(blockX, blockY, blockIndicies);
      FinLocation fins[27] = {0};
      int finCount = 0;
      FinLocation nonFins[27] ={0};
      int nonFinCount = 0;
      for(int b = 0; b < fishSize; ++b) {
        int baseIndex = context->potentialBaseCombo->bases[b].baseIndex;
        int coverIndicies[9] = {0};
        int coverCount = maskToIntArray(context->potentialBaseCombo->bases[b].indicies, coverIndicies);
        for(int c = 0; c < coverCount; ++c) {
          int coverIndex = coverIndicies[c];
          bool locatedInBlock = context->byRow ? isLocationInBlock(baseIndex, coverIndex, i) : isLocationInBlock(coverIndex, baseIndex, i);
          if(locatedInBlock) {
            fins[finCount].row = context->byRow ? baseIndex : coverIndex;
            fins[finCount].col = context->byRow ? coverIndex : baseIndex;
            finCount++;
          } else {
            nonFins[finCount].row = context->byRow ? baseIndex : coverIndex;
            nonFins[finCount].col = context->byRow ? coverIndex : baseIndex;
            nonFinCount++;
          }
        }
      }

      if(finCount == 0) continue;
      
      uint16_t nonFinCovers = 0;
      for(int f = 0; f < nonFinCount; ++f ) {
        nonFinCovers |= 1 << context->byRow ? nonFins[f].col : nonFins[f].row;
      }
      int nonFinCoverIndicies[9] = {0};
      int nonFinCountCoverCount = maskToIntArray(nonFinCovers, nonFinCoverIndicies);
      if(nonFinCountCoverCount != fishSize) continue;

      for(int nf = 0; nf < nonFinCountCoverCount; ++nf) {
        for(int j = 0; j < PUZZLE_WIDTH; ++j) {
          bool insideBox = context->byRow ? isLocationInBlock(j, nonFinCoverIndicies[nf], i) : isLocationInBlock(nonFinCoverIndicies[nf], j, i);
          if(!insideBox) continue;
          bool inBaseRow = false;
          for(int b = 0; b < fishSize; ++b) {
            if(j == context->potentialBaseCombo->bases[b].baseIndex) {
              inBaseRow = true;
            }
            if(inBaseRow) break;
          }
          if(inBaseRow) continue;
          
          int cellIndex = context->byRow ? (j * PUZZLE_WIDTH + nonFinCoverIndicies[nf]) : (j + PUZZLE_WIDTH * nonFinCoverIndicies[nf]);
          if(hasCandidate(context->allCandidates[cellIndex], context->candidate)) {
            int len = context->indiciesToRemoveCount;
            context->indiciesToRemove[len] = cellIndex;
            context->indiciesToRemoveCount++;
          }
        }
      }
      if(context->indiciesToRemoveCount > 0) {
        return true;
      }
    }
    return false;
  }

  for(int i = startIndex; i < context->housesWithCandidateCount; ++i) {
    context->potentialBaseCombo->bases[depth] = context->allFishBases->bases[i];
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
  uint16_t colsWithCandidate = 0;
  uint16_t candidateMask = 1 << (candidate - 1);
  uint16_t candidateCol[9] = {0};
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    getCandidateCol(i, puzzleCandidates, candidateCol);
    for(int k = 0; k < PUZZLE_WIDTH; ++k) {
      if(hasCandidate(candidateCol[k], candidate)) {
        colsWithCandidate |= candidateMask;
        break;
      }
    }
  }
  return colsWithCandidate;
}

void getBlockIndicies(int blockX, int blockY, int* blockIndices) {
  int blockXOffset = blockX * BLOCK_WIDTH;
  int blockYOffset = blockY * BLOCK_WIDTH * PUZZLE_WIDTH;
  int startIndex = blockXOffset + blockYOffset;

  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    int rowModifier = (i / BLOCK_WIDTH) * PUZZLE_WIDTH;
    int colModifier = i % BLOCK_WIDTH;
    int cellIndex = startIndex + rowModifier + colModifier;
    blockIndices[i] = cellIndex;
  }
}

