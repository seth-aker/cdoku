#include "strategies.h";
#include <string.h>
void getStrategyName(Strategy strategy, char* string) {
  switch (strategy) {
  case FULL_HOUSE:
    strcpy(string, "Full House");
    break;
  case NAKED_SINGLE:
    strcpy(string, "Naked Single");
    break;
  case HIDDEN_SINGLE:
    strcpy(string, "Hidden Single");
    break;
  case LOCKED_CANDIDATE_POINTING:
    strcpy(string, "Locked Candidate Pointing");
    break;
  case LOCKED_CANDIDATE_CLAIMING:
    strcpy(string, "Locked Candidate Claiming");
    break;
  case HIDDEN_PAIRS:
    strcpy(string, "Hidden Pair");
    break;
  case NAKED_PAIRS:
    strcpy(string, "Naked Pair");
    break;
  case HIDDEN_TRIPLES:
    strcpy(string, "Hidden Triple");
    break;
  case NAKED_TRIPLES:
    strcpy(string, "Naked Triple");
    break;
  case HIDDEN_QUADS:
    strcpy(string, "Hidden Quad");
    break;
  case NAKED_QUADS:
    strcpy(string, "Naked Quad");
    break;
  case GUESS:
    strcpy(string, "Guess");
    break;
  default:
    strcpy(string, "None");
    break;
  }
}