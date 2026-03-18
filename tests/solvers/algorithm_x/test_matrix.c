#include "unity.h"
#include "matrix.h"
#include "log.c"

#include <stdio.h>
void setUp(void) {

}

void tearDown(void) {

}

void test_initMatrix_operates_on_all_nodes(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current = &matrix.columns[0];

  TEST_ASSERT_EQUAL_INT(MAX_NODES, matrix_node_counter);
}

void test_initMatrix_created_doubly_linked_column_list(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current = &matrix.columns[0];


  for (int i = 0; i <= TOTAL_COLUMNS; ++i) {
    current = current->right;
  }
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&matrix.columns[0], current, "current and head should be the same after traversing right");

  for (int i = 0; i <= TOTAL_COLUMNS; ++i) {
    current = current->left;
  }
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&matrix.columns[0], current, "current and head should be the same after traversing left");

}

void test_initMatrix_assigns_nine_rows_to_each_column(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  int expected_header_size = 9;

  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current_header = &matrix.columns[0];

  for(int i = 0; i < TOTAL_COLUMNS; ++i) {
    TEST_ASSERT_EQUAL_INT(expected_header_size, current_header->size);
    Node* current_row = current_header->down;
    for(int j = 0; j < PUZZLE_WIDTH; ++j) {
      TEST_ASSERT_EQUAL_PTR_MESSAGE(current_header, current_row->column, "each row node should point to its column header node");
      current_row = current_row->down;
    }
    TEST_ASSERT_EQUAL_PTR_MESSAGE(current_header, current_row, "After tranversing down 9 times, should loop back to the header");

    current_row = current_header->up;
    for(int j = 0; j < PUZZLE_WIDTH; ++j) {
      current_row = current_row->up;
    }
    TEST_ASSERT_EQUAL_PTR_MESSAGE(current_header, current_row, "After traversing up 9 times, should loop back to the header");

    current_header = current_header->right;
  }
}

void test_initMatrix_correctly_assigns_row_lookup_ptrs(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current_header = &matrix.columns[0];


  for(int i = 0; i < TOTAL_ROWS; ++i) {
    TEST_ASSERT_NOT_NULL(matrix.row_lookup[i]);
    TEST_ASSERT_EQUAL_INT_MESSAGE(i, matrix.row_lookup[i]->row_id, "Row ids should match location index in row_lookup array");
  }
}

void test_initMatrix_correctly_connects_the_row_nodes_horizontally(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  initMatrix(&matrix, matrix_node_pool, &matrix_node_counter);

  for(int i = 0; i < TOTAL_ROWS; ++i) {
    Node* row_node = matrix.row_lookup[i];
    int row_id = row_node->row_id;
    for(int r = 0; r < 4; ++r) {
      TEST_ASSERT_EQUAL_INT_MESSAGE(row_id, row_node->row_id, "horizontally connected nodes should have the same row_id");
      row_node = row_node->right;
    }
    TEST_ASSERT_EQUAL_PTR_MESSAGE(matrix.row_lookup[i], row_node->right, "horizonally linked nodes should loop back to starting node going right");
    
    row_node = matrix.row_lookup[i];
    for(int r = 0; r < 4; ++r) {
      row_node = row_node->left;
    }
    TEST_ASSERT_EQUAL_PTR_MESSAGE(matrix.row_lookup[i], row_node->left, "horizontally linked nodes should loop back to starting node going left");
  }

}

void test_getMinCol_correctly_returns_smallest_col_size(void) {
  Node root, c1, c2, c3, c4;
  root.left = &c4; root.right = &c1;
  c1.left = &root; c1.right = &c2; 
  c2.left = &c1; c2.right = &c3;
  c3.left = &c2; c3.right = &c4;
  c4.left = &c3; c4.right = &root;


  c1.size = 4;
  c2.size = 9;
  c3.size = 2;
  c4.size = 9;

  Node* result = getMinCol(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c3, result, "c3 has the smallest size and should be returned by getMinCol()");

  c1.size = 0;
  c2.size = 5;
  c3.size = 9;
  c4.size = 9;
  result = getMinCol(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c1, result, "c1 should return when it has a value of 0");
}

void test_getMinCol_returns_first_of_cols_with_equal_size(void) {
  Node root, c1, c2, c3, c4;
  root.left = &c4; root.right = &c1;
  c1.left = &root; c1.right = &c2; 
  c2.left = &c1; c2.right = &c3;
  c3.left = &c2; c3.right = &c4;
  c4.left = &c3; c4.right = &root;

  c1.size = 5;
  c2.size = 5;
  c3.size = 5;
  c4.size = 5;
  
  Node* result = getMinCol(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c1, result, "First of equally sized cols should be returned");
}

void test_getMinCol_returns_root_when_puzzle_solved(void) {
  Node root;
  root.right = &root;
  root.size = 0;
  Node* result = getMinCol(&root);
  
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&root, result, "root should be returned if no colunms are attached");

}

void test_getMinCol_returns_correct_col_when_only_one_option(void) {
  Node root, c1;
  root.right = &c1;
  c1.right = &root;
  root.left = &c1;
  c1.left = &root;

  c1.size = 1;
  Node* result = getMinCol(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c1, result, "c1 should be returned when it is the only col left.");
}
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initMatrix_operates_on_all_nodes);
  RUN_TEST(test_initMatrix_created_doubly_linked_column_list);
  RUN_TEST(test_initMatrix_assigns_nine_rows_to_each_column);
  RUN_TEST(test_initMatrix_correctly_assigns_row_lookup_ptrs);
  RUN_TEST(test_getMinCol_correctly_returns_smallest_col_size);
  RUN_TEST(test_getMinCol_returns_first_of_cols_with_equal_size);
  RUN_TEST(test_getMinCol_returns_root_when_puzzle_solved);
  RUN_TEST(test_getMinCol_returns_correct_col_when_only_one_option);
  return UNITY_END();
}
