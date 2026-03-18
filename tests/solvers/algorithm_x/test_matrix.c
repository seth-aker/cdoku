#include "unity.h"
#include "matrix.h"
#include "log.c"

#include <stdio.h>
void setUp(void) {
}

void tearDown(void) {

}

void test_initMatrix(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current = &matrix.columns[0];

  TEST_ASSERT_EQUAL_INT(MAX_NODES, matrix_node_counter);
  for (int i = 0; i < TOTAL_COLUMNS; ++i) {
    TEST_ASSERT_EQUAL_INT(9, current->size);
    current = current->right;
  }
  UNITY_TEST_ASSERT_EQUAL_PTR(current, &matrix.columns[0], 23, "current and head should be the same");

}
void test_getMinCol(void) {

}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initMatrix);
  return UNITY_END();
}
