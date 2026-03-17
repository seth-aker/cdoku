#include "unity.h"
#include "matrix.h"
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

  TEST_ASSERT_EQUAL_INT(MAX_NODES, matrix_node_counter);


}
void test_getMinCol(void) {

}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initMatrix);
  return UNITY_END();
}