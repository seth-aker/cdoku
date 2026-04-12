#ifndef SRC_SOLVERS_LOGICAL_SUBSETS_H
#define SRC_SOLVERS_LOGICAL_SUBSETS_H
#include "puzzle.h"
#include "step.h"

TechniqueResult search_naked_pairs(Puzzle* puzzle);
TechniqueResult search_naked_triples(Puzzle* puzzle);
TechniqueResult search_naked_quads(Puzzle* puzzle);

TechniqueResult find_naked_pair(Puzzle* puzzle, House* house);
TechniqueResult find_naked_triple(Puzzle* puzzle, House* house);
TechniqueResult find_naked_quad(Puzzle* puzzle, House* house);

TechniqueResult search_hidden_pairs(Puzzle* puzzle);
TechniqueResult search_hidden_triples(Puzzle* puzzle);
TechniqueResult search_hidden_quads(Puzzle* Puzzle);

TechniqueResult find_hidden_pair(Puzzle* puzzle, House* house);
TechniqueResult find_hidden_triple(Puzzle* puzzle, House* house);
TechniqueResult find_hidden_quad(Puzzle* puzzle, House* house);

#endif // SRC_SOLVERS_LOGICAL_SUBSETS_H
