#include "strategies.h"
#include "string.h"

int getStrategyScore(Strategy strategy) {
  switch (strategy) {
    case FULL_HOUSE:
      return 100;
    case NAKED_SINGLE:
      return 100;
    case HIDDEN_SINGLE:
      return 125;
    case LOCKED_CANDIDATE_POINTING:
      return 180;
    case LOCKED_CANDIDATE_CLAIMING:
      return 220;
    case HIDDEN_PAIRS:
      return 350;
    case NAKED_PAIRS: 
      return 250;
    case HIDDEN_TRIPLES:
      return 600;
    case NAKED_TRIPLES:
      return 450;
    case NAKED_QUADS:
      return 800;
    case HIDDEN_QUADS:
      return 1200;
    case X_WING: 
      return 350;
    case FINNED_X_WING:
      return 550;
    case SWORDFISH:
      return 700;
    case FINNED_SWORDFISH:
      return 900;
    case JELLYFISH: 
      return 1500;
    case FINNED_JELLYFISH:
      return 2500;
    case SKYSCRAPER:
      return 450;
    case XY_WING:
      return 500;
    case GUESS:
      return 5000;
    default:
      return 0;
  }
}
// The string passed to this function must be a minimum of 27 characters long.
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
  case X_WING:
    strcpy(string, "X-Wing");
    break;
  case FINNED_X_WING:
    strcpy(string, "Finned X-Wing");
    break;
  case SWORDFISH: 
    strcpy(string, "Swordfish");
    break;
  case FINNED_SWORDFISH:
    strcpy(string, "Finned Swordfish");
    break;
  case JELLYFISH: 
    strcpy(string, "Jellyfish");
    break;
  case FINNED_JELLYFISH:
    strcpy(string, "Finned Jellyfish");
    break;
  case GUESS:
    strcpy(string, "Guess");
    break;
  default:
    strcpy(string, "None");
    break;
  }
}