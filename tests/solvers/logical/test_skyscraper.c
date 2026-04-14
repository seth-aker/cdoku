#include "unity.h"
#include "skyscraper.h" // Replace with your actual header
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
// ROW-BASED SKYSCRAPER TESTS
// ==========================================

void test_find_skyscrapers_rows_actionable(void) {
  // Target: Candidate 9 (0x0100)
  // Base Rows: Row 2 and Row 6
  // Shared Base Column: Col 5 
  // Roofs: Row 2 is at Col 2. Row 6 is at Col 0.
  // Base cells: (2,5)=23, (6,5)=59
  // Roof cells: (2,2)=20, (6,0)=54
  uint16_t mask = 0x0100;

  // 1. Clear Candidate 9 from ALL of Row 2 and Row 6
  for(int i = 0; i < 9; i++) {
    remove_candidate(ROW_TO_IDXS[2][i], mask);
    remove_candidate(ROW_TO_IDXS[6][i], mask);
  }

  // 2. Put Candidate 9 back in our Skyscraper formation
  p.candidates[ROW_TO_IDXS[2][5]] |= mask; // Base
  p.candidates[ROW_TO_IDXS[6][5]] |= mask; // Base
  p.candidates[ROW_TO_IDXS[2][2]] |= mask; // Roof 1
  p.candidates[ROW_TO_IDXS[6][0]] |= mask; // Roof 2

  // Execution
  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_skyscrapers(&p));

  // Assertions:
  // The "Kill Zone" contains cells that see BOTH Roof 1 (2,2) and Roof 2 (6,8).
  // Cell (6,2) [idx 56] sees Roof 1 via its column, and Roof 2 via its row.
  // Cell (2,8) [idx 26] sees Roof 1 via its row, and Roof 2 via its column.

  TEST_ASSERT_EQUAL_HEX16(0x00FF, p.candidates[0]); // 0x01FF minus 0x0100
  TEST_ASSERT_EQUAL_HEX16(0x00FF, p.candidates[9]);

  // Ensure the actual Skyscraper was unharmed
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[20]);
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[54]);
}

// ==========================================
// COLUMN-BASED SKYSCRAPER TESTS
// ==========================================

void test_find_skyscrapers_cols_actionable(void) {
  // Target: Candidate 4 (0x0008)
  // Base Cols: Col 1 and Col 7
  // Shared Base Row: Row 4
  // Roofs: Col 1 is at Row 0. Col 7 is at Row 8.
  // Base cells: (4,1)=37, (4,7)=43
  // Roof cells: (0,1)=1,  (1,7)=79
  uint16_t mask = 0x0008;

  // 1. Clear Candidate 4 from ALL of Col 1 and Col 7
  for(int i = 0; i < 9; i++) {
    remove_candidate(COL_TO_IDXS[1][i], mask);
    remove_candidate(COL_TO_IDXS[7][i], mask);
  }

  // 2. Put Candidate 4 back in our Skyscraper formation
  p.candidates[37] |= mask; // Base
  p.candidates[43] |= mask; // Base
  p.candidates[1] |= mask; // Roof 1
  p.candidates[16] |= mask; // Roof 2

  // Execution
  TEST_ASSERT_EQUAL(PROGRESS_MADE, find_skyscrapers(&p));

  // Assertions:
  // The "Kill Zone" contains cells that see BOTH Roof 1 (0,1) and Roof 2 (8,7).
  // Cell (8,1) [idx 73] sees Roof 1 via col, and Roof 2 via row.
  // Cell (0,7) [idx 7] sees Roof 1 via row, and Roof 2 via col.

  TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[6]); // 0x01FF minus 0x0008
  TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[8]);
  TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[9]);
  TEST_ASSERT_EQUAL_HEX16(0x01F7, p.candidates[11]);

  // Ensure the actual Skyscraper was unharmed
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[1]);
  TEST_ASSERT_EQUAL_HEX16(0x01FF, p.candidates[16]);
}

// ==========================================
// STAGNATION TEST
// ==========================================

void test_find_skyscrapers_stagnates(void) {
  // Setup a valid Row Skyscraper exactly like the first test
  uint16_t mask = 0x0100;
  for(int i = 0; i < 9; i++) {
    remove_candidate(ROW_TO_IDXS[2][i], mask);
    remove_candidate(ROW_TO_IDXS[6][i], mask);
  }
  p.candidates[23] |= mask;
  p.candidates[59] |= mask;
  p.candidates[20] |= mask;
  p.candidates[62] |= mask;

  // BUT manually erase the candidate from the target kill zones
  // This simulates a Skyscraper that has already been exhausted
  remove_candidate(56, mask); // (6,2)
  remove_candidate(26, mask); // (2,8)

  // Must return NO_PROGRESS to prevent infinite solving loops
  TEST_ASSERT_EQUAL(NO_PROGRESS, find_skyscrapers(&p));
}

// ==========================================
// TEST RUNNER
// ==========================================
int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_find_skyscrapers_rows_actionable);
  RUN_TEST(test_find_skyscrapers_cols_actionable);
  RUN_TEST(test_find_skyscrapers_stagnates);

  return UNITY_END();
}
