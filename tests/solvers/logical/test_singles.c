#include "unity.h"
#include "singles.h"
#include "puzzle.h"
#include "string.h"
Puzzle p;

void setUp(void) {
  memset(p.cells, 0, sizeof(uint8_t) * TOTAL_CELLS);
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    p.candidates[i] = (uint8_t)0x01FF;
  }
}

void tearDown(void) {

}
void test_is_full_house_row_true(void) {
  for(uint8_t i = 0; i < 8; i++) {
    p.cells[i] = i + 1;
  }

  TEST_ASSERT_TRUE(is_full_house(&p, 8));

}
void test_is_full_house_col_true(void) {
  for(int i = 0; i < 8; i++) {
    p.cells[i * 9] = i + 1;
  }
  TEST_ASSERT_TRUE(is_full_house(&p, 72));
}

void test_is_full_house_block_true(void) {
  for(int i = 0; i < 8; i++) {
    p.cells[BLOCK_TO_IDXS[0][i]] = i + 1;
  }
  TEST_ASSERT_TRUE(is_full_house(&p, BLOCK_TO_IDXS[0][8]));
}

void test_is_full_house_false(void) {
  TEST_ASSERT_FALSE(is_full_house(&p, 0));
}

void test_find_naked_single_success(void) {
  p.candidates[10] = 0x0010;

  TechiqueResult result = find_naked_single(&p);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  TEST_ASSERT_EQUAL_UINT8(5, p.cells[10]);
}

void test_find_naked_single_none_found(void) {
  TechiqueResult result = find_naked_single(&p);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

void test_find_hidden_single_in_row_success(void) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i != 3) {
      p.candidates[i] &= ~0x0040; // Remove bit 6 from all other cells in Row 0
    }
  }

  TechiqueResult result = find_hidden_single(&p);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
}

void test_find_hidden_single_in_col_success(void) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i != 3) {
      p.candidates[i * 9] &= ~0x0040;
    }
  }
  TechiqueResult result = find_hidden_single(&p);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
}

void test_find_hidden_single_in_block_success(void) {
  for(int i = 0; i < PUZZLE_WIDTH; ++i) {
    if(i != 3) {
      p.candidates[BLOCK_TO_IDXS[0][i]] &= ~0x0040;
    }
  }
  TechiqueResult result = find_hidden_single(&p);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
}
void test_find_hidden_single_invalid_state(void) {
  for(int i = 1; i < PUZZLE_WIDTH; ++i) {
    p.candidates[i] &= ~0x00C0; // Remove both bits (0x0040 | 0x0080) from other cells
  }

  TechiqueResult result = find_hidden_single(&p);

  TEST_ASSERT_EQUAL(INVALID_STATE, result);
}

void test_find_hidden_single_none_found(void) {
  TechiqueResult result = find_hidden_single(&p);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

int main(void) {
  UNITY_BEGIN();

  // is_full_house tests
  RUN_TEST(test_is_full_house_row_true);
  RUN_TEST(test_is_full_house_col_true);
  RUN_TEST(test_is_full_house_block_true);
  RUN_TEST(test_is_full_house_false);

  // find_naked_single tests
  RUN_TEST(test_find_naked_single_success);
  RUN_TEST(test_find_naked_single_none_found);

  // find_hidden_single tests
  RUN_TEST(test_find_hidden_single_in_row_success);
  RUN_TEST(test_find_hidden_single_in_col_success);
  RUN_TEST(test_find_hidden_single_in_block_success);
  RUN_TEST(test_find_hidden_single_invalid_state);
  RUN_TEST(test_find_hidden_single_none_found);

  return UNITY_END();
}
