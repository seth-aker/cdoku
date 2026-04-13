#include "unity.h"
#include "x_wing.h" // Replace with your actual header
#include "puzzle_globals.h"
#include <string.h>

// ==========================================
// GLOBAL TEST STATE
// ==========================================
Puzzle p;

void remove_candidate(int index, uint16_t mask) {
  p.candidates[index] &= ~mask;
}

void setUp(void) {
  memset(p.cells, 0, sizeof(p.cells));
  // Reset all 81 cells to empty, containing all 9 candidates (Bits 0-8)
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    p.candidates[i] = 0x01FF;
  }
}

void tearDown(void) {
  // Nothing to tear down
}

// ==========================================
// ROW-BASED X-WING TESTS
// ==========================================

void test_find_x_wing_rows_actionable(void) {
  // Target: Candidate 1 (0x0001)
  // Base Rows: Row 1 and Row 5
  // Cover Columns: Col 2 and Col 7
  // Intersections: (R1,C2)=11, (R1,C7)=16, (R5,C2)=47, (R5,C7)=52
  uint16_t mask = 0x0001;

  // 1. Clear Candidate 1 from ALL of Row 1 and Row 5
  for(int i = 0; i < 9; i++) {
    remove_candidate(ROW_TO_IDXS[1][i], mask);
    remove_candidate(ROW_TO_IDXS[5][i], mask);
  }

  // 2. Put Candidate 1 back ONLY in the four corners of our X-Wing
  p.candidates[11] |= mask;
  p.candidates[16] |= mask;
  p.candidates[47] |= mask;
  p.candidates[52] |= mask;

  // Execution
  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_x_wing(&p));

  // Assertions:
  // It should have swept Cover Columns 2 and 7 and erased Candidate 1.
  // Let's check Row 0, which intersects those columns at indices 2 and 7.
  TEST_ASSERT_EQUAL_HEX16(0x01FE, p.candidates[2]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, p.candidates[7]);

  // Ensure the corners of the X-Wing were NOT erased (they are protected)
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[11]);
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[47]);
}

// ==========================================
// COLUMN-BASED X-WING TESTS
// ==========================================

void test_find_x_wing_cols_actionable(void) {
  // Target: Candidate 2 (0x0002)
  // Base Cols: Col 1 and Col 8
  // Cover Rows: Row 3 and Row 7
  // Intersections: (R3,C1)=28, (R3,C8)=35, (R7,C1)=64, (R7,C8)=71
  uint16_t mask = 0x0002;

  // 1. Clear Candidate 2 from ALL of Col 1 and Col 8
  for(int i = 0; i < 9; i++) {
    remove_candidate(COL_TO_IDXS[1][i], mask);
    remove_candidate(COL_TO_IDXS[8][i], mask);
  }

  // 2. Put Candidate 2 back ONLY in the four corners
  p.candidates[28] |= mask;
  p.candidates[35] |= mask;
  p.candidates[64] |= mask;
  p.candidates[71] |= mask;

  // Execution
  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_x_wing(&p));

  // Assertions:
  // It should have swept Cover Rows 3 and 7 and erased Candidate 2.
  // Let's check Col 0, which intersects those rows at indices 27 and 63.
  TEST_ASSERT_EQUAL_HEX16(0x01FD, p.candidates[27]);
  TEST_ASSERT_EQUAL_HEX16(0x01FD, p.candidates[63]);

  // Ensure the corners of the X-Wing were NOT erased
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[28]);
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[71]);
}

// ==========================================
// STAGNATION & HELPER TESTS
// ==========================================

void test_find_x_wing_stagnates(void) {
  // Setup a valid Row X-Wing exactly like the first test
  uint16_t mask = 0x0001;
  for(int i = 0; i < 9; i++) {
    remove_candidate(ROW_TO_IDXS[1][i], mask);
    remove_candidate(ROW_TO_IDXS[5][i], mask);
  }
  p.candidates[11] |= mask; p.candidates[16] |= mask;
  p.candidates[47] |= mask; p.candidates[52] |= mask;

  // BUT manually erase the candidate from the target elimination zones (Col 2 & Col 7)
  // This simulates an X-Wing that was already found in a previous pass
  for(int r = 0; r < 9; r++) {
    if(r == 1 || r == 5) continue; // Skip the base rows
    remove_candidate(ROW_TO_IDXS[r][2], mask);
    remove_candidate(ROW_TO_IDXS[r][7], mask);
  }

  // Must return NO_PROGRESS to prevent infinite solving loops
  TEST_ASSERT_EQUAL(NO_PROGRESS, find_x_wing(&p));
}

void test_count_candidate_in_house_logic(void) {
  // Test the helper function directly
  XWing_Context ctx;
  ctx.candidate_mask = 0x0004; // Candidate 3
  ctx.base_house_count = 0; // Let's act like this is the first house found

  // We will pass Row 0. Let's make candidate 3 only exist in cell 2 and cell 6
  uint16_t mask = 0x0004;
  for(int i = 0; i < 9; i++) {
    remove_candidate(ROW_TO_IDXS[0][i], mask);
  }
  p.candidates[2] |= mask;
  p.candidates[6] |= mask;

  int count = count_candidate_in_house(&p, ROW_TO_IDXS[0], &ctx, 2);

  TEST_ASSERT_EQUAL(2, count);
  // It should have recorded the relative column positions (2 and 6)
  TEST_ASSERT_EQUAL(2, ctx.cover_houses[0][0]);
  TEST_ASSERT_EQUAL(6, ctx.cover_houses[0][1]);
}

// ==========================================
// TEST RUNNER
// ==========================================
int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_find_x_wing_rows_actionable);
  RUN_TEST(test_find_x_wing_cols_actionable);
  RUN_TEST(test_find_x_wing_stagnates);
  RUN_TEST(test_count_candidate_in_house_logic);

  return UNITY_END();
}
