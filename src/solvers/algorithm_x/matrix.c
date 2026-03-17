#include <stdbool.h>

#include "matrix.h"

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
void findSolutions(Node* root_node, int* solution_count, int solution_max) {
  if (root_node == root_node->right) {
    (*solution_count)++;
    return;
  }

  Node* chosen_col = getMinCol(root_node); // get the column with the fewest potential solutions

  if(chosen_col->size == 0) {
    return;
  }

  cover(chosen_col);

  Node* row = chosen_col->down;
  while(row != chosen_col) {
    Node* right_node = row->right;
    while (right_node != row) {
      cover(right_node->column);
      right_node = right_node->right;
    }

    findSolutions(root_node, solution_count, solution_max);
    
    if(*solution_count > solution_max) {
      return;
    }
    Node* left_node = row->left;
    while (left_node != row) {
      uncover(left_node->column);
      left_node = left_node->left;
    }
    row = row->down;
  }
  uncover(chosen_col);
}

void convertPuzzleToMatrix(int cells[], Matrix* matrix) {
  for (int i = 0; i < TOTAL_CELLS; ++i) {
    if (cells[i] == 0) {
      continue;
    }
    int r = i / PUZZLE_WIDTH;
    int c = i % PUZZLE_WIDTH;

    int row_id = (ROW_OFFSET * r) + (c * PUZZLE_WIDTH) + (cells[i] - 1);

    Node* clue_node = matrix->row_lookup[row_id];

    cover(clue_node->column);

    Node* right_node = clue_node->right;
    while (right_node != clue_node) {
      cover(right_node->column);
      right_node = right_node->right;
    }
  }
}

void initMatrix(Matrix* matrix, Node node_pool[], int* node_pool_counter) {
  matrix->columns[0] = matrix->head;
  matrix->columns[0].column = &matrix->head;
  matrix->columns[0].up = &matrix->columns[0];
  matrix->columns[0].down = &matrix->columns[0];
  matrix->columns[0].size = 0;
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
          row_nodes[i] = &node_pool[(*node_pool_counter)++];
          row_nodes[i]->row_id = row_id;

          Node* header_col = &matrix->columns[col_idx[i]];
          row_nodes[i]->column = header_col;
          row_nodes[i]->up = header_col->up;
          row_nodes[i]->down = header_col;

          header_col->up->down = row_nodes[i];
          header_col->up = row_nodes[i];
          header_col->size++;
        }
        matrix->row_lookup[row_id] = row_nodes[0]; // this allows O(1) lookup time for nodes by row_id
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

Node* getMinCol(Node* root) {
  Node* min_col = root->right;
  Node* current = root->right;
  int min_size = 9999;
  while(current != root) {
    if(min_size > current->size) {
      min_size = current->size;
      min_col = current;
    }
    if(min_size == 0) { // short circuit
      return min_col;
    }
    current = current->right;
  }
  return min_col;
}
