#include "unity.h"
#include "puzzle.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_isValidNumInCell_returns_true_for_valid_numbers(void) {
  uint8_t cells[81] = {0};
  uint8_t num = 1;
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    TEST_ASSERT_TRUE_MESSAGE(isValidNumInCell(num, i, cells), "Expected isValidNumInCell to return true");
  }

  uint8_t filled_cells[81] = {
    0,2,3,4,5,6,7,8,9,
    4,5,6,7,8,9,1,2,3,
    7,8,9,1,2,3,4,5,6,
    2,3,4,5,6,7,8,9,1,
    5,6,7,8,9,1,2,3,4,
    8,9,1,2,3,4,5,6,7,
    3,4,5,6,7,8,9,1,2,
    6,7,8,9,1,2,3,4,5,
    9,1,2,3,4,5,6,7,8
  }; 
  TEST_ASSERT_TRUE_MESSAGE(isValidNumInCell(num, 0, filled_cells), "Expected isValidNumInCell ot return true");
}

void test_isValidNumInCell_returns_false_for_invalid_numbers(void) {
  uint8_t cells[81] = {
    0,2,3,4,5,6,7,8,9,
    4,5,6,7,8,9,1,2,3,
    7,8,9,1,2,3,4,5,6,
    2,3,4,5,6,7,8,9,1,
    5,6,7,8,9,1,2,3,4,
    8,9,1,2,3,0,0,6,7,
    3,4,5,6,7,8,9,1,2,
    6,7,8,9,1,2,3,4,5,
    0,1,2,0,4,0,6,7,0
  };
  TEST_ASSERT_FALSE_MESSAGE(isValidNumInCell(9, 0, cells), "Expected 9 in idx 0 to return as an invalid number (test row)");
  TEST_ASSERT_FALSE_MESSAGE(isValidNumInCell(3, 80, cells), "Expected 3 in idx 80 to return as an invalid number (test col)");
  TEST_ASSERT_FALSE_MESSAGE(isValidNumInCell(5, 50, cells), "Expected 5 in idx 50 to return as an invalid number (test blocks)");

}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_isValidNumInCell_returns_true_for_valid_numbers);
  RUN_TEST(test_isValidNumInCell_returns_false_for_invalid_numbers);
  return UNITY_END();
}