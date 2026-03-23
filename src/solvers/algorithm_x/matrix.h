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

int getColValForCell(int row, int col);
int getColValForRow(int row, int col, int num);
int getColValForCol(int row, int col, int num);
int getColValForBlock(int row, int col, int num);
void findSolutions(Node* root_node, int* solution_count, int solution_max);

void convertPuzzleToMatrix(uint8_t cells[], Matrix* matrix);
void initMatrix(Matrix* matrix, Node node_pool[], int* matrix_node_counter);
Node* getMinCol(Node* root);
#endif // SRC_SOLVERS_ALGORITHM_X_H
