#include "unity.h"
#include "locked_candidates.h" 
#include <string.h>

Puzzle p;

// Helper to easily erase specific candidates during test setup
void remove_candidate(int index, uint8_t mask) {
  p.candidates[index] &= ~mask;
}

void setUp(void) {
  memset(p.cells, 0, sizeof(p.cells));

  // Reset all 81 cells to empty, containing all 9 candidates (Bits 0-8 -> 0x01FF)
  for(int i = 0; i < TOTAL_CELLS; ++i) {
    p.candidates[i] = 0x01FF;
  }
}

void tearDown(void) {
  // Nothing to tear down
}


void test_find_locked_candidate_pointing_row_actionable(void) {
  // Goal: Make Candidate 1 (0x0001) "point" along Row 0 inside Block 0.
  // Block 0 consists of rows 0, 1, 2 and cols 0, 1, 2.
  // We remove 0x0001 from Row 1 and Row 2 of Block 0.
  int block_0_row_1[] = { 9, 10, 11 };
  int block_0_row_2[] = { 18, 19, 20 };

  for(int i = 0; i < 3; i++) {
    remove_candidate(block_0_row_1[i], 0x0001);
    remove_candidate(block_0_row_2[i], 0x0001);
  }

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_locked_candidate_pointing(&p));

  // The solver should have erased 0x0001 from the rest of Row 0 (Indices 3 through 8)
  for(int i = 3; i <= 8; i++) {
    TEST_ASSERT_EQUAL_HEX16(0x01FE, p.candidates[i]); // 0x01FF minus 0x0001
  }
}

void test_find_locked_candidate_pointing_col_actionable(void) {
  // Goal: Make Candidate 2 (0x0002) "point" along Col 0 inside Block 0.
  // We remove 0x0002 from Col 1 and Col 2 of Block 0.
  int block_0_col_1[] = { 1, 10, 19 };
  int block_0_col_2[] = { 2, 11, 20 };

  for(int i = 0; i < 3; i++) {
    remove_candidate(block_0_col_1[i], 0x0002);
    remove_candidate(block_0_col_2[i], 0x0002);
  }

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_locked_candidate_pointing(&p));

  // The solver should have erased 0x0002 from the rest of Col 0 (Indices 27, 36, 45, 54, 63, 72)
  for(int r = 3; r < 9; r++) {
    int col_0_idx = r * 9;
    TEST_ASSERT_EQUAL_HEX16(0x01FD, p.candidates[col_0_idx]); // 0x01FF minus 0x0002
  }
}

void test_find_locked_candidate_pointing_stagnates(void) {
  // Set up the same pointing row as before...
  int block_0_row_1[] = { 9, 10, 11 };
  int block_0_row_2[] = { 18, 19, 20 };
  for(int i = 0; i < 3; i++) {
    remove_candidate(block_0_row_1[i], 0x0001);
    remove_candidate(block_0_row_2[i], 0x0001);
  }

  // ...But also manually erase the candidate from the target elimination zone.
  // This simulates a pointing line that has already been solved.
  for(int i = 3; i <= 8; i++) {
    remove_candidate(i, 0x0001);
  }

  // The function should recognize the pointing line, but return NO_PROGRESS 
  // because there are no candidates left to actually delete.
  TEST_ASSERT_EQUAL(NO_PROGRESS, find_locked_candidate_pointing(&p));
}

// ==========================================
// CLAIMING CANDIDATES (BOX-LINE) TESTS
// ==========================================

void test_find_locked_candidate_claiming_row_actionable(void) {
  // Goal: Make Candidate 3 (0x0004) "claim" Block 0 from Row 0.
  // This happens when a candidate in a row ONLY exists inside one block.
  // We remove 0x0004 from the rest of Row 0 (Indices 3 through 8).
  for(int i = 3; i <= 8; i++) {
    remove_candidate(i, 0x0004);
  }

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_locked_candidate_claiming(&p));

  // The solver should have erased 0x0004 from the rest of Block 0
  int block_0_row_1[] = { 9, 10, 11 };
  int block_0_row_2[] = { 18, 19, 20 };
  for(int i = 0; i < 3; i++) {
    TEST_ASSERT_EQUAL_HEX16(0x01FB, p.candidates[block_0_row_1[i]]); // 0x01FF minus 0x0004
    TEST_ASSERT_EQUAL_HEX16(0x01FB, p.candidates[block_0_row_2[i]]);
  }
}

void test_find_locked_candidate_claiming_col_actionable(void) {
  // Goal: Make Candidate 4 (0x0008) "claim" Block 0 from Col 0.
  // We remove 0x0008 from the rest of Col 0 outside of Block 0.
  for(int r = 3; r < 9; r++) {
    int col_0_idx = r * 9;
    remove_candidate(col_0_idx, 0x0008);
  }

  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_locked_candidate_claiming(&p));

  // The solver should have erased 0x0008 from the rest of Block 0
  int block_0_col_1[] = { 1, 10, 19 };
  int block_0_col_2[] = { 2, 11, 20 };
  for(int i = 0; i < 3; i++) {
    TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[block_0_col_1[i]]); // 0x01FF minus 0x0008
    TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[block_0_col_2[i]]);
  }
}

void test_find_locked_candidate_claiming_stagnates(void) {
  // Set up the claiming row...
  for(int i = 3; i <= 8; i++) {
    remove_candidate(i, 0x0004);
  }

  // ...But also manually erase the candidate from the target elimination zone.
  int block_0_row_1[] = { 9, 10, 11 };
  int block_0_row_2[] = { 18, 19, 20 };
  for(int i = 0; i < 3; i++) {
    remove_candidate(block_0_row_1[i], 0x0004);
    remove_candidate(block_0_row_2[i], 0x0004);
  }

  // Must return NO_PROGRESS to prevent infinite loops on solved claiming lines
  TEST_ASSERT_EQUAL(NO_PROGRESS, find_locked_candidate_claiming(&p));
}

void test_remove_pointing_row_actionable(void) {
  // Target: Block 4 (Center Block), Local Row 1 (Global Row 4)
  // Global Row 4 indices: 36, 37, 38, [39, 40, 41 - inside block 4], 42, 43, 44
  uint8_t mask = 0x0001; // Candidate 1

  // The function should erase the mask from the cells OUTSIDE the block.
  // Ensure they have the candidate to begin with:
  p.candidates[36] = 0x01FF;
  p.candidates[44] = 0x01FF;

  // Call helper directly: Block 4, Local Row 1
  bool result = remove_pointing_row(&p, mask, 4, 1);

  TEST_ASSERT_TRUE(result);
  // Verify it stripped the candidate outside the block
  TEST_ASSERT_EQUAL_HEX16(0x01FE, p.candidates[36]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, p.candidates[44]);
  // Verify it left the candidates INSIDE the block alone
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[39]);
}

void test_remove_pointing_row_stagnates(void) {
  // Target: Block 4, Local Row 1
  uint8_t mask = 0x0001;

  // Simulate a board where the candidate is already missing from the outside cells
  p.candidates[36] &= ~mask;
  p.candidates[37] &= ~mask;
  p.candidates[38] &= ~mask;
  p.candidates[42] &= ~mask;
  p.candidates[43] &= ~mask;
  p.candidates[44] &= ~mask;

  bool result = remove_pointing_row(&p, mask, 4, 1);

  TEST_ASSERT_FALSE(result); // No progress should be made
}

// ==========================================
// REMOVE POINTING COL TESTS
// ==========================================

void test_remove_pointing_col_actionable(void) {
  // Target: Block 4 (Center Block), Local Col 2 (Global Col 5)
  // Global Col 5 indices: 5, 14, 23, [32, 41, 50 - inside block 4], 59, 68, 77
  uint8_t mask = 0x0002; // Candidate 2

  // Call helper directly: Block 4, Local Col 2
  bool result = remove_pointing_col(&p, mask, 4, 2);

  TEST_ASSERT_TRUE(result);
  // Verify it stripped the candidate outside the block
  TEST_ASSERT_EQUAL_HEX16(0x01FD, p.candidates[5]);
  TEST_ASSERT_EQUAL_HEX16(0x01FD, p.candidates[77]);
  // Verify it left the candidates INSIDE the block alone
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[41]);
}

void test_remove_pointing_col_stagnates(void) {
  uint8_t mask = 0x0002;
  int outside_indices[] = { 5, 14, 23, 59, 68, 77 };

  for(int i = 0; i < 6; i++) {
    p.candidates[outside_indices[i]] &= ~mask;
  }

  bool result = remove_pointing_col(&p, mask, 4, 2);
  TEST_ASSERT_FALSE(result);
}

// ==========================================
// REMOVE CLAIMING ROW TESTS
// ==========================================

void test_remove_claiming_row_actionable(void) {
  // Target: Block 0 (Top Left Block). We want a claiming line in Local Row 1.
  // Therefore, we must SKIP Local Row 1 (indices 3, 4, 5 of the block array)
  // and ELIMINATE from Local Row 0 (0, 1, 2) and Local Row 2 (6, 7, 8).

  // Block 0 global indices:
  // Row 0: 0, 1, 2
  // Row 1: 9, 10, 11 (These should be skipped/protected)
  // Row 2: 18, 19, 20

  uint8_t mask = 0x0004; // Candidate 3

  // Call helper directly: Block 0, skip_row_start = 3
  bool result = remove_claiming_row(&p, mask, 0, 3);

  TEST_ASSERT_TRUE(result);

  // Check eliminated rows
  TEST_ASSERT_EQUAL_HEX16(0x01FB, p.candidates[0]);  // Local Row 0
  TEST_ASSERT_EQUAL_HEX16(0x01FB, p.candidates[19]); // Local Row 2

  // Check skipped/protected row
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[10]); // Local Row 1
}

void test_remove_claiming_row_stagnates(void) {
  uint8_t mask = 0x0004;

  // Erase candidate from the target elimination zones
  p.candidates[0] &= ~mask; p.candidates[1] &= ~mask; p.candidates[2] &= ~mask;
  p.candidates[18] &= ~mask; p.candidates[19] &= ~mask; p.candidates[20] &= ~mask;

  bool result = remove_claiming_row(&p, mask, 0, 3);
  TEST_ASSERT_FALSE(result);
}

// ==========================================
// REMOVE CLAIMING COL TESTS
// ==========================================

void test_remove_claiming_col_actionable(void) {
  // Target: Block 0 (Top Left Block). Claiming line in Local Col 0.
  // We must SKIP Local Col 0 (offsets 0, 3, 6)
  // We must ELIMINATE from Local Col 1 (offsets 1, 4, 7) and Local Col 2 (offsets 2, 5, 8).

  // Block 0 global indices:
  // Col 0: 0, 9, 18 (These should be skipped/protected)
  // Col 1: 1, 10, 19
  // Col 2: 2, 11, 20

  uint8_t mask = 0x0008; // Candidate 4

  // Call helper directly: Block 0, skip_col_idx = 0
  bool result = remove_claiming_col(&p, mask, 0, 0);

  TEST_ASSERT_TRUE(result);

  // Check eliminated columns
  TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[1]);  // Local Col 1
  TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[20]); // Local Col 2

  // Check skipped/protected column
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[9]);  // Local Col 0
}

void test_remove_claiming_col_stagnates(void) {
  uint8_t mask = 0x0008;

  // Erase candidate from the target elimination zones
  p.candidates[1] &= ~mask; p.candidates[10] &= ~mask; p.candidates[19] &= ~mask;
  p.candidates[2] &= ~mask; p.candidates[11] &= ~mask; p.candidates[20] &= ~mask;

  bool result = remove_claiming_col(&p, mask, 0, 0);
  TEST_ASSERT_FALSE(result);
}

// ==========================================
// TEST RUNNER
// ==========================================
int main(void) {
  UNITY_BEGIN();

  // Pointing Lines
  RUN_TEST(test_find_locked_candidate_pointing_row_actionable);
  RUN_TEST(test_find_locked_candidate_pointing_col_actionable);
  RUN_TEST(test_find_locked_candidate_pointing_stagnates);

  // Claiming Lines (Box-Line Reduction)
  RUN_TEST(test_find_locked_candidate_claiming_row_actionable);
  RUN_TEST(test_find_locked_candidate_claiming_col_actionable);
  RUN_TEST(test_find_locked_candidate_claiming_stagnates);

  RUN_TEST(test_remove_pointing_row_actionable);
  RUN_TEST(test_remove_pointing_row_stagnates);
  RUN_TEST(test_remove_pointing_col_actionable);
  RUN_TEST(test_remove_pointing_col_stagnates);

  // Claiming Helpers
  RUN_TEST(test_remove_claiming_row_actionable);
  RUN_TEST(test_remove_claiming_row_stagnates);
  RUN_TEST(test_remove_claiming_col_actionable);
  RUN_TEST(test_remove_claiming_col_stagnates);
  return UNITY_END();
}
