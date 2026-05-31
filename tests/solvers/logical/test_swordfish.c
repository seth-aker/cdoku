#include "unity.h"
#include <string.h>
#include "swordfish.h"
#include "puzzle.h"
#include "utils.h"

static Puzzle test_puzzle;
static const uint16_t TARGET_CAND = 0x0001; // Candidate 1

extern void apply_step(Puzzle* puzzle, Step step);

static void clear_target_candidate(void) {
  for(int i = 0; i < 81; i++) {
    test_puzzle.candidates[i] &= ~TARGET_CAND;
  }
}

static void place_cand(int row, int col) {
  test_puzzle.candidates[row * 9 + col] |= TARGET_CAND;
}

void setUp(void) {
  for(int i = 0; i < 81; i++) {
    test_puzzle.cells[i] = 0;
    test_puzzle.candidates[i] = 0x01FF;
  }
}

void tearDown(void) {}

// =============================================================================
// HORIZONTAL SWORDFISH TESTS (Base: Rows 1, 4, 7 | Cover: Cols 2, 5, 8)
// Targets: Row 0 Col 2, Row 8 Col 8
// =============================================================================

void test_horizontal_swordfish_3_3_3(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5); place_cand(1, 8); // R1: 3
  place_cand(4, 2); place_cand(4, 5); place_cand(4, 8); // R4: 3
  place_cand(7, 2); place_cand(7, 5); place_cand(7, 8); // R7: 3

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_horizontal_swordfish_2_2_2(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5);                   // R1: 2
  place_cand(4, 5); place_cand(4, 8);                   // R4: 2
  place_cand(7, 2);                   place_cand(7, 8); // R7: 2

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_horizontal_swordfish_2_3_3(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5);                   // R1: 2
  place_cand(4, 2); place_cand(4, 5); place_cand(4, 8); // R4: 3
  place_cand(7, 2); place_cand(7, 5); place_cand(7, 8); // R7: 3

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_horizontal_swordfish_2_2_3(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5);                   // R1: 2
  place_cand(4, 5); place_cand(4, 8);                   // R4: 2
  place_cand(7, 2); place_cand(7, 5); place_cand(7, 8); // R7: 3

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_horizontal_swordfish_2_3_2(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5);                   // R1: 2
  place_cand(4, 2); place_cand(4, 5); place_cand(4, 8); // R4: 3
  place_cand(7, 5); place_cand(7, 8);                   // R7: 2

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_horizontal_swordfish_3_2_2(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5); place_cand(1, 8); // R1: 3
  place_cand(4, 2); place_cand(4, 5);                   // R4: 2
  place_cand(7, 5); place_cand(7, 8);                   // R7: 2

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_horizontal_swordfish_3_3_2(void) {
  clear_target_candidate();
  place_cand(1, 2); place_cand(1, 5); place_cand(1, 8); // R1: 3
  place_cand(4, 2); place_cand(4, 5); place_cand(4, 8); // R4: 3
  place_cand(7, 5); place_cand(7, 8);                   // R7: 2

  place_cand(0, 2); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[0 * 9 + 2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

// =============================================================================
// VERTICAL SWORDFISH TESTS (Base: Cols 1, 4, 7 | Cover: Rows 2, 5, 8)
// Targets: Row 2 Col 0, Row 8 Col 8
// =============================================================================

void test_vertical_swordfish_3_3_3(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1); place_cand(8, 1); // C1: 3
  place_cand(2, 4); place_cand(5, 4); place_cand(8, 4); // C4: 3
  place_cand(2, 7); place_cand(5, 7); place_cand(8, 7); // C7: 3

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_vertical_swordfish_2_2_2(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1);                   // C1: 2
  place_cand(5, 4); place_cand(8, 4);                   // C4: 2
  place_cand(2, 7);                   place_cand(8, 7); // C7: 2

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_vertical_swordfish_2_3_3(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1);                   // C1: 2
  place_cand(2, 4); place_cand(5, 4); place_cand(8, 4); // C4: 3
  place_cand(2, 7); place_cand(5, 7); place_cand(8, 7); // C7: 3

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_vertical_swordfish_2_2_3(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1);                   // C1: 2
  place_cand(5, 4); place_cand(8, 4);                   // C4: 2
  place_cand(2, 7); place_cand(5, 7); place_cand(8, 7); // C7: 3

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_vertical_swordfish_2_3_2(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1);                   // C1: 2
  place_cand(2, 4); place_cand(5, 4); place_cand(8, 4); // C4: 3
  place_cand(5, 7); place_cand(8, 7);                   // C7: 2

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_vertical_swordfish_3_2_2(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1); place_cand(8, 1); // C1: 3
  place_cand(2, 4); place_cand(5, 4);                   // C4: 2
  place_cand(5, 7); place_cand(8, 7);                   // C7: 2

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

void test_vertical_swordfish_3_3_2(void) {
  clear_target_candidate();
  place_cand(2, 1); place_cand(5, 1); place_cand(8, 1); // C1: 3
  place_cand(2, 4); place_cand(5, 4); place_cand(8, 4); // C4: 3
  place_cand(5, 7); place_cand(8, 7);                   // C7: 2

  place_cand(2, 0); place_cand(8, 8); // Targets

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_swordfish(&test_puzzle));
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[2 * 9 + 0]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[8 * 9 + 8]);
}

// =============================================================================
// TEST RUNNER
// =============================================================================
int main(void) {
  UNITY_BEGIN();

  // Horizontal Suites
  RUN_TEST(test_horizontal_swordfish_3_3_3);
  RUN_TEST(test_horizontal_swordfish_2_2_2);
  RUN_TEST(test_horizontal_swordfish_2_3_3);
  RUN_TEST(test_horizontal_swordfish_2_2_3);
  RUN_TEST(test_horizontal_swordfish_2_3_2);
  RUN_TEST(test_horizontal_swordfish_3_2_2);
  RUN_TEST(test_horizontal_swordfish_3_3_2);

  // Vertical Suites
  RUN_TEST(test_vertical_swordfish_3_3_3);
  RUN_TEST(test_vertical_swordfish_2_2_2);
  RUN_TEST(test_vertical_swordfish_2_3_3);
  RUN_TEST(test_vertical_swordfish_2_2_3);
  RUN_TEST(test_vertical_swordfish_2_3_2);
  RUN_TEST(test_vertical_swordfish_3_2_2);
  RUN_TEST(test_vertical_swordfish_3_3_2);

  return UNITY_END();
}
