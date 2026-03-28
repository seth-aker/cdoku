#ifndef SRC_SOLVERS_LOGICAL_SUBSETS_H
#define SRC_SOLVERS_LOGICAL_SUBSETS_H
#include "puzzle.h"
#include "step.h"

TechiqueResult search_naked_pairs(Puzzle* puzzle);
TechiqueResult search_naked_triples(Puzzle* puzzle);
TechiqueResult search_naked_quads(Puzzle* puzzle);

TechiqueResult find_naked_pair(Puzzle* puzzle, House* house);
TechiqueResult find_naked_triple(Puzzle* puzzle, House* house);
TechiqueResult find_naked_quad(Puzzle* puzzle, House* house);

TechiqueResult search_hidden_pairs(Puzzle* puzzle);
TechiqueResult search_hidden_triples(Puzzle* puzzle);
TechiqueResult search_hidden_quads(Puzzle* Puzzle);

TechiqueResult find_hidden_pair(Puzzle* puzzle, House* house);
TechiqueResult find_hidden_triple(Puzzle* puzzle, House* house);
TechiqueResult find_hidden_quad(Puzzle* puzzle, House* house);

#endif // SRC_SOLVERS_LOGICAL_SUBSETS_H
