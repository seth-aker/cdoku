#include "unity.h"
#include "node.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_cover_horizontally_unlinks_column(void) {
  Node root, col1, col2, node1, node2;

  root.left = &col2; root.right = &col1;
  col1.left = &root; col1.right = &col2;
  col2.left = &col1; col2.right = &root;

  col1.down = &node1; col1.up = &node1;
  node1.up = &col1; node1.down = &col1;
  node1.column = &col1;

  col2.down = &node2; col2.up = &node2;
  node2.up = &col2; node2.down = &col2;
  node2.column = &col2;

  col1.size = 1; col2.size = 1;

  node1.left = &node2; node1.right = &node2;
  node2.left = &node1; node2.right = &node1;

  cover(&col1);

  TEST_ASSERT_EQUAL_PTR_MESSAGE(root.right, &col2, "Expexted col1 to be disconnected from the columns");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(root.left, &col2, "Expexted col1 to be disconnected from the columns");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.right, &root, "Expexted col1 to be disconnected from the columns");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.left, &root, "Expexted col1 to be disconnected from the columns");


  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.right, &col2, "Expected col1 right pointer to remain pointed at col2");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.left, &root, "Expected col1 left to pointer to remain pointed at root");

}

void test_cover_vertically_unlinks_nodes(void) {
  Node root, col1, col2, node1, node2;

  root.left = &col2; root.right = &col1;
  col1.left = &root; col1.right = &col2;
  col2.left = &col1; col2.right = &root;

  col1.down = &node1; col1.up = &node1;
  node1.up = &col1; node1.down = &col1;
  node1.column = &col1;

  col2.down = &node2; col2.up = &node2;
  node2.up = &col2; node2.down = &col2;
  node2.column = &col2;

  col1.size = 1; col2.size = 1;

  node1.left = &node2; node1.right = &node2;
  node2.left = &node1; node2.right = &node1;

  cover(&col1);

  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.down, &col2, "Col2's node should be covered and it should point to itself vertically");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.up, &col2, "Col2's node should be unlinked and col2 should point to itself vertically");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(node2.up, &node2, "Node2 should still point up to it's col");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(node2.down, &col2, "Node2 should still point down to it's col");

}

void test_cover_should_correctly_track_col_size(void) {
  Node root, col1, col2, node1, node2;

  root.left = &col2; root.right = &col1;
  col1.left = &root; col1.right = &col2;
  col2.left = &col1; col2.right = &root;

  col1.down = &node1; col1.up = &node1;
  node1.up = &col1; node1.down = &col1;
  node1.column = &col1;

  col2.down = &node2; col2.up = &node2;
  node2.up = &col2; node2.down = &col2;
  node2.column = &col2;

  col1.size = 1; col2.size = 1;

  node1.left = &node2; node1.right = &node2;
  node2.left = &node1; node2.right = &node1;

  cover(&col1);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, col2.size, "Col2 size should be 0 when node is unlinked.");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, col1.size, "Col1's size should not change when covered.");

}
void test_uncover_should_correctly_connect_uncovered_col(void) {
  Node root, col1, col2, node1, node2;

  root.left = &col2; root.right = &col2;
  col1.left = &root; col1.right = &col2;
  col2.left = &root; col2.right = &root;

  col1.down = &node1; col1.up = &node1;
  node1.up = &col1; node1.down = &col1;
  node1.column = &col1;

  col2.up = &col2; col2.down = &col2;
  node2.up = &col2; node2.down = &col2;
  node2.column = &col2;

  col1.size = 1; col2.size = 0;

  node1.left = &node2; node1.right = &node2;
  node2.left = &node1; node2.right = &node1;

  uncover(&col1);

  TEST_ASSERT_EQUAL_PTR_MESSAGE(root.right, &col1, "Root right should point to uncovered col1");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.left, &col1, "Col1 left should point to uncovered col1");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.right, &col2, "Col1 right should point to col2");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.left, &root, "Col1 right should point to root");

}
void test_uncover_should_correctly_connect_vertical_nodes(void) {
  Node root, col1, col2, node1, node2;

  root.left = &col2; root.right = &col2;
  col1.left = &root; col1.right = &col2;
  col2.left = &root; col2.right = &root;

  col1.down = &node1; col1.up = &node1;
  node1.up = &col1; node1.down = &col1;
  node1.column = &col1;

  col2.up = &col2; col2.down = &col2;
  node2.up = &col2; node2.down = &col2;
  node2.column = &col2;

  col1.size = 1; col2.size = 0;

  node1.left = &node2; node1.right = &node2;
  node2.left = &node1; node2.right = &node1;

  uncover(&col1);

  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.down, &node1, "Col1 down shouuld point to node1");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.up, &node1, "Col1 up point to should be node1");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.down, &node2, "Col2 down should point to node2");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(col2.up, &node2, "Col2 up should point to node2");

  TEST_ASSERT_EQUAL_PTR_MESSAGE(node1.up, &col1, "Node1 up should point to col1");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(node1.down, &col1, "Node1 down should point to col1");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(node2.up, &col2, "Node2 up should point to col2");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(node2.down, &col2, "Node2 down should point to col2");


}
void test_uncover_should_update_col_size(void) {
  Node root, col1, col2, node1, node2;

  root.left = &col2; root.right = &col2;
  col1.left = &root; col1.right = &col2;
  col2.left = &root; col2.right = &root;

  col1.down = &node1; col1.up = &node1;
  node1.up = &col1; node1.down = &col1;
  node1.column = &col1;

  col2.up = &col2; col2.down = &col2;
  node2.up = &col2; node2.down = &col2;
  node2.column = &col2;

  col1.size = 1; col2.size = 0;

  node1.left = &node2; node1.right = &node2;
  node2.left = &node1; node2.right = &node1;

  uncover(&col1);

  TEST_ASSERT_EQUAL_PTR_MESSAGE(col1.size, 1, "Col1 size should be 1");
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_cover_horizontally_unlinks_column);
  RUN_TEST(test_cover_vertically_unlinks_nodes);
  RUN_TEST(test_cover_should_correctly_track_col_size);
  RUN_TEST(test_uncover_should_correctly_connect_uncovered_col);
  RUN_TEST(test_uncover_should_correctly_connect_vertical_nodes);
  RUN_TEST(test_uncover_should_update_col_size);
  UNITY_END();
}
