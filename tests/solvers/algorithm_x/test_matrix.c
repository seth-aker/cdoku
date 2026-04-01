#include "unity.h"
#include "matrix.h"
#include "log.c"

#include <stdbool.h>
#include <stdio.h>

int count_active_cols(Matrix* matrix) {
  int count = 0;
  Node* current = matrix->root.right;
  while(current != &matrix->root) {
    count++;
    current = current->right;
  }
  return count;
}

bool is_col_active(Matrix* matrix, Node* target_col) {
  Node* current = matrix->root.right;
  while(current != &matrix->root) {
    if(current == target_col) {
      return true;
    }
    current = current->right;
  }
  return false;
}

void link_vertical(Node* header, Node* node) {
  node->column = header;
  node->up = header->up;
  node->down = header;
  header->up->down = node;
  header->up = node;
  header->size++;
}

void link_horizontal(Node* a, Node* b) {
  a->right = b; a->left = b;
  b->right = a; b->left = a;
}

void setUp(void) {

}

void tearDown(void) {

}

void test_initMatrix_operates_on_all_nodes(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current = &matrix.columns[0];

  TEST_ASSERT_EQUAL_INT(MAX_NODES, matrix_node_counter);
}

void test_initMatrix_created_doubly_linked_column_list(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  Node* current = &matrix.columns[0];


  for(int i = 0; i <= TOTAL_COLUMNS; ++i) {
    current = current->right;
  }
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&matrix.columns[0], current, "current and head should be the same after traversing right");

  for(int i = 0; i <= TOTAL_COLUMNS; ++i) {
    current = current->left;
  }
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&matrix.columns[0], current, "current and head should be the same after traversing left");

}

void test_initMatrix_assigns_nine_rows_to_each_column(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  int expected_header_size = 9;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
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

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
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

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);

  for(int i = 0; i < TOTAL_ROWS; ++i) {
    Node* row_node = matrix.row_lookup[i];
    int row_id = row_node->row_id;
    for(int r = 0; r < 4; ++r) {
      TEST_ASSERT_EQUAL_INT_MESSAGE(row_id, row_node->row_id, "horizontally connected nodes should have the same row_id");
      row_node = row_node->right;
    }
    row_node = row_node->right;
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

  Node* result = get_min_col(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c3, result, "c3 has the smallest size and should be returned by getMinCol()");

  c1.size = 0;
  c2.size = 5;
  c3.size = 9;
  c4.size = 9;
  result = get_min_col(&root);
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

  Node* result = get_min_col(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c1, result, "First of equally sized cols should be returned");
}

void test_getMinCol_returns_root_when_puzzle_solved(void) {
  Node root;
  root.right = &root;
  root.size = 0;
  Node* result = get_min_col(&root);

  TEST_ASSERT_EQUAL_PTR_MESSAGE(&root, result, "root should be returned if no colunms are attached");

}

void test_getMinCol_returns_correct_col_when_only_one_option(void) {
  Node root, c1;
  root.right = &c1;
  c1.right = &root;
  root.left = &c1;
  c1.left = &root;

  c1.size = 1;
  Node* result = get_min_col(&root);
  TEST_ASSERT_EQUAL_PTR_MESSAGE(&c1, result, "c1 should be returned when it is the only col left.");
}

void test_convertPuzzleToMatrix_should_cover_correct_cols(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  TEST_ASSERT_EQUAL_INT(TOTAL_COLUMNS, count_active_cols(&matrix));

  uint8_t puzzle_cells[81] = { 0 };
  puzzle_cells[0] = 1;

  convert_puzzle_to_matrix(puzzle_cells, &matrix);
  TEST_ASSERT_EQUAL_INT_MESSAGE(320, count_active_cols(&matrix), "4 constraints should be covered.");
}

void test_convertPuzzleToMatrix_should_not_change_matrix_with_empty_puzzle(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  TEST_ASSERT_EQUAL_INT(TOTAL_COLUMNS, count_active_cols(&matrix));

  uint8_t puzzle_cells[81] = { 0 };
  convert_puzzle_to_matrix(puzzle_cells, &matrix);
  TEST_ASSERT_EQUAL_INT_MESSAGE(324, count_active_cols(&matrix), "should have all cols");
}

void test_covertPuzzleToMatrix_covers_specific_constraints(void) {
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;
  Matrix matrix;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  TEST_ASSERT_EQUAL_INT(TOTAL_COLUMNS, count_active_cols(&matrix));

  uint8_t puzzle_cells[81] = { 0 };
  puzzle_cells[0] = 5;

  convert_puzzle_to_matrix(puzzle_cells, &matrix);
  TEST_ASSERT_MESSAGE(is_col_active(&matrix, &matrix.columns[0]) == false, "Cell Constraint: (0 * 9) + 0 = 0");
  TEST_ASSERT_MESSAGE(is_col_active(&matrix, &matrix.columns[85]) == false, "Row Constraint: 81 + (0 * 9) + 4 = 85");
  TEST_ASSERT_MESSAGE(is_col_active(&matrix, &matrix.columns[166]) == false, "Col Constraint: 162 + (0 * 9) + 4 = 166");
  TEST_ASSERT_MESSAGE(is_col_active(&matrix, &matrix.columns[247]) == false, "Block Constraint: 243 + (0 * 9) + 4 = 247");

  TEST_ASSERT_MESSAGE(is_col_active(&matrix, &matrix.columns[1]), "Other cols are still active");
}

void test_findSolutions_returns_solution_count_1_when_solution_is_reached(void) {
  Node root;
  root.right = &root;
  int solution_count = 0;
  int solution_max = 1;

  find_solutions(&root, &solution_count, solution_max);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, solution_count, "Expected solution count to be 1 when root node points to itself.");
}

void test_findSolutions_returns_2_when_two_solutions_exist(void) {
  Node root, colA, colB, colC;
  root.left = &colC; root.right = &colA; root.up = &root; root.down = &root;
  colA.left = &root; colA.right = &colB; colA.up = &colA; colB.down = &colB;
  colB.left = &colA; colB.right = &colC; colB.up = &colB; colB.down = &colB;
  colC.left = &colB; colC.right = &root; colC.up = &colC; colC.down = &colC;
  colA.size = 0; colB.size = 0; colC.size = 0;

  Node r1A, r2b, r2c, r3a, r3b, r4c;

  link_vertical(&colA, &r1A);
  r1A.left = &r1A; r1A.right = &r1A;

  link_vertical(&colB, &r2b);
  link_vertical(&colC, &r2c);
  link_horizontal(&r2b, &r2c);

  link_vertical(&colA, &r3a);
  link_vertical(&colB, &r3b);
  link_horizontal(&r3a, &r3b);

  link_vertical(&colC, &r4c);
  r4c.left = &r4c; r4c.right = &r4c;

  int solution_count = 0;
  int solution_max = 3;

  find_solutions(&root, &solution_count, solution_max);

  TEST_ASSERT_EQUAL_INT_MESSAGE(2, solution_count, "Expected 2 solutions to be found");
}
void test_findSolutions_short_circuits_after_finding_2_solutions(void) {
  Matrix matrix;
  Node matrix_node_pool[MAX_NODES];
  int matrix_node_counter = 0;

  init_matrix(&matrix, matrix_node_pool, &matrix_node_counter);
  // This array will yield exactly 6 valid solutions
  uint8_t six_solution_puzzle[81] = {
      5, 0, 4,  6, 7, 8,  9, 0, 0,
      6, 7, 0,  0, 9, 5,  0, 4, 8,
      0, 9, 8,  0, 4, 0,  5, 6, 7,

      8, 5, 9,  7, 6, 0,  4, 0, 0,
      4, 0, 6,  8, 5, 0,  7, 9, 0,
      7, 0, 0,  9, 0, 4,  8, 5, 6,

      9, 6, 0,  5, 0, 7,  0, 8, 4,
      0, 8, 7,  4, 0, 9,  6, 0, 5,
      0, 4, 5,  0, 8, 6,  0, 7, 9
  };
  convert_puzzle_to_matrix(six_solution_puzzle, &matrix);

  int solution_count = 0;
  find_solutions(&matrix.root, &solution_count, 1);
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, solution_count, "Expected findSolutions() to short-circuit at puzzle count of 2 even though total solutions is 6.");
  TEST_ASSERT_EQUAL_INT_MESSAGE(108, count_active_cols(&matrix), "Expect matrix to be intact.");

  convert_puzzle_to_matrix(six_solution_puzzle, &matrix);

  solution_count = 0;
  find_solutions(&matrix.root, &solution_count, 100);

  TEST_ASSERT_EQUAL_INT_MESSAGE(18, solution_count, "Expected findSolutions() to find exactly 6 solutions and stop.");
  TEST_ASSERT_EQUAL_INT_MESSAGE(108, count_active_cols(&matrix), "Expected matrix to be intact.");
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initMatrix_operates_on_all_nodes);
  RUN_TEST(test_initMatrix_created_doubly_linked_column_list);
  RUN_TEST(test_initMatrix_assigns_nine_rows_to_each_column);
  RUN_TEST(test_initMatrix_correctly_assigns_row_lookup_ptrs);
  RUN_TEST(test_initMatrix_correctly_connects_the_row_nodes_horizontally);
  RUN_TEST(test_getMinCol_correctly_returns_smallest_col_size);
  RUN_TEST(test_getMinCol_returns_first_of_cols_with_equal_size);
  RUN_TEST(test_getMinCol_returns_root_when_puzzle_solved);
  RUN_TEST(test_getMinCol_returns_correct_col_when_only_one_option);

  RUN_TEST(test_convertPuzzleToMatrix_should_cover_correct_cols);
  RUN_TEST(test_convertPuzzleToMatrix_should_not_change_matrix_with_empty_puzzle);
  RUN_TEST(test_covertPuzzleToMatrix_covers_specific_constraints);

  RUN_TEST(test_findSolutions_returns_solution_count_1_when_solution_is_reached);
  RUN_TEST(test_findSolutions_returns_2_when_two_solutions_exist);
  RUN_TEST(test_findSolutions_short_circuits_after_finding_2_solutions);

  return UNITY_END();
}
