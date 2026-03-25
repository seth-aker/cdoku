#ifndef SRC_SOLVERS_ALGORITHM_X_MATRIX_H
#define SRC_SOLVERS_ALGORITHM_X_MATRIX_H
#include "node.h"
#include "puzzle.h"

#define ROW_OFFSET 81
#define COL_OFFSET 162
#define BLOCK_OFFSET 243
#define TOTAL_COLUMNS 324
#define TOTAL_ROWS 729
#define MAX_NODES 2916

typedef struct Matrix {
  Node root;
  Node columns[TOTAL_COLUMNS];
  Node* row_lookup[TOTAL_ROWS];
} Matrix;

int get_col_val_for_cell(int row, int col);
int get_col_val_for_row(int row, int col, int num);
int get_col_val_for_col(int row, int col, int num);
int get_col_val_for_block(int row, int col, int num);
void find_solutions(Node* root_node, int* solution_count, int solution_max);

void convert_puzzle_to_matrix(uint8_t cells[], Matrix* matrix);
void init_matrix(Matrix* matrix, Node node_pool[], int* matrix_node_counter);
Node* get_min_col(Node* root);
#endif // SRC_SOLVERS_ALGORITHM_X_H
