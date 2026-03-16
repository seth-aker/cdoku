#ifndef SRC_SOLVERS_ALGORITHM_X_NODE_H
#define SRC_SOLVERS_ALGORITHM_X_NODE_H

#include "string.h"

typedef struct Node {
  struct Node* left, * right, * up, * down, * column;
  int row_id;
  int size;
} Node;


void cover(Node* column);
void uncover(Node* column);

#endif // SRC_SOLVERS_ALGORITHM_X_H
