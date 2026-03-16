#include "node.h"
#include "stdbool.h"



void cover(Node* column) {
  column->right->left = column->left;
  column->left->right = column->right;

  Node* i = column->down;
  while (i != column) {
    Node* j = i->right;
    while (j != i) {
      j->down->up = j->up;
      j->up->down = j->down;
      j->column->size -= 1;
      j = j->right;
    }
    i = i->down;
  }
}

void uncover(Node* column) {
  Node* i = column->up;
  while (i != column) {
    Node* j = i->left;
    while (j != i) {
      j->column->size += 1;
      j->down->up = j;
      j->up->down = j;
      j = j->left;
    }
    i = i->up;
  }
  column->right->left = column;
  column->left->right = column;
}

// bool search(int depth, Node* root, )
