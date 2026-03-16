#include "matrix.h";

Node* getNewNode() {
  return &memory_pool[nodeCounter++];
}
int getColValForCell(int row, int col) {
  return (row * 9) + col;
}
int getColValForRow(int row, int col, int num) {
  int n = num - 1;
  return ROW_OFFSET + (row * 9) + n;
}
int getColValForCol(int row, int col, int num) {
  int n = num - 1;
  return COL_OFFSET + (col * 9) + n;
}
int getColValForBlock(int row, int col, int num) {
  int blockIndex = (row / 3) * 3 + (col / 3);
  int n = num - 1;
  return BLOCK_OFFSET + (blockIndex * 9) + n;
}

void convertPuzzleToMatrix(int* cells, Matrix* matrix) {

}

void initMatrix(Matrix* matrix) {
  matrix->columns[0] = matrix->head;
  matrix->columns[0].column = &matrix->head;
  matrix->columns[0].up = &matrix->columns[0];
  matrix->columns[0].down = &matrix->columns[0];
  Node* current = &matrix->head;
  for (int i = 1; i < TOTAL_COLUMNS; ++i) {
    matrix->columns[i].left = &matrix->columns[i - 1];
    matrix->columns[i].up = &matrix->columns[i];
    matrix->columns[i].down = &matrix->columns[i];
    matrix->columns[i].size = 0;
    matrix->columns[i].column = &matrix->columns[i];
    current->right = &matrix->columns[i];
    current = &matrix->columns[i];
  }
  current->right = &matrix->columns[0];
  matrix->columns[0].left = current;

  for (int r = 0; r < PUZZLE_WIDTH; ++r) {
    for (int c = 0; c < PUZZLE_WIDTH; ++c) {
      for (int n = 0; n < PUZZLE_WIDTH; ++n) {
        int row_id = (r * 81) + (c * 9) + n;

        int col_idx[4];
        col_idx[0] = getColValForCell(r, c);
        col_idx[1] = getColValForRow(r, c, n);
        col_idx[2] = getColValForCol(r, c, n);
        col_idx[3] = getColValForBlock(r, c, n);

        Node* row_nodes[4];
        for (int i = 0; i < 4; ++i) {
          row_nodes[i] = getNewNode();
          row_nodes[i]->row_id = row_id;

          Node* header_col = &matrix->columns[col_idx[i]];
          row_nodes[i]->column = header_col;
          row_nodes[i]->up = header_col->up;
          row_nodes[i]->down = header_col;

          header_col->up->down = row_nodes[i];
          header_col->up = row_nodes[i];
          header_col->size++;
        }

        for (int i = 0; i < 4; ++i) {
          int prev = (i == 0) ? 3 : i + 1;
          int next = (i == 3) ? 0 : i - 1;

          row_nodes[i]->right = row_nodes[next];
          row_nodes[i]->left = row_nodes[prev];
        }
      }
    }
  }
}

