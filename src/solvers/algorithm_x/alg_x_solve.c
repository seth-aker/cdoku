#include "alg_x_solve.h"
#include "matrix.h"
#include "puzzle.h"
#include <stdbool.h>


bool algorithm_x_has_unique_sol(Puzzle* puzzle) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;

  Matrix matrix;
  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);
  convertPuzzleToMatrix(puzzle->cells, &matrix);

  int solution_count = 0;
  findSolutions(&matrix.head, &solution_count, 1);

  if(solution_count != 1) {
    return false;
  }
  return true;
}