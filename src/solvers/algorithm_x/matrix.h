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

extern Node matrix_node_pool[MAX_NODES];
extern int matrix_node_counter;

typedef struct Matrix {
  Node head;
  Node columns[TOTAL_COLUMNS];
} Matrix;

Node* getNewNode();
int getColValForCell(int row, int col);
int getColValForRow(int row, int col, int num);
int getColValForCol(int row, int col, int num);
int getColValForBlock(int row, int col, int num);

void convertPuzzleToMatrix(int* cells, Matrix* matrix);
void initMatrix(Matrix* matrix);
#endif // SRC_SOLVERS_ALGORITHM_X_H
