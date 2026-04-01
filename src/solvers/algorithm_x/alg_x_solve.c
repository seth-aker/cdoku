#include "alg_x_solve.h"
#include "matrix.h"
#include "puzzle.h"
#include <stdbool.h>


bool algorithm_x_has_unique_sol(uint8_t cells[]) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;

  Matrix matrix;
  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  convert_puzzle_to_matrix(cells, &matrix);

  int solution_count = 0;
  find_solutions(&matrix.root, &solution_count, 1);

  if(solution_count != 1) {
    return false;
  }
  return true;
}
