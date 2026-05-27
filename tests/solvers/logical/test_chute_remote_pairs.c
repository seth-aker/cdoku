#include "unity.h"
#include "chute_remote_pairs.h"
#include "puzzle.h"
#include "utils.h"

static Puzzle test_puzzle;

// Forward declaration of the step applicator to track state changes in tests.
// Assumes standard integration where apply_step removes the eliminated_mask from the target_cell.
extern void apply_step(Puzzle* puzzle, Step step);

void setUp(void) {
  // Initialize a blank puzzle before each test
  for(int i = 0; i < 81; i++) {
    test_puzzle.cells[i] = 0;
    test_puzzle.candidates[i] = 0x01FF; // Bits 0-8 active (Candidates 1-9)
  }
}

void tearDown(void) {
  // Clean up if necessary
}

// -----------------------------------------------------------------------------
// Test 1: Horizontal Chute Remote Pair - 0 Overlap
// Expectation: Eliminates both candidates from the intersection of the two cells.
// -----------------------------------------------------------------------------
void test_horizontal_chute_zero_overlap_eliminates_both(void) {
  // Cell 1: Row 0, Col 0 (Band 0, Stack 0). Mask: 0x03 (Candidates 1, 2)
  test_puzzle.candidates[0] = 0x03;

  // Cell 2: Row 1, Col 3 (Band 0, Stack 1). Mask: 0x03 (Candidates 1, 2)
  test_puzzle.candidates[12] = 0x03;

  // Target block: Band 0, Stack 2 -> Rows 0,1,2 and Cols 6,7,8
  // Unused Row: 2. Unused Col block: 6, 7, 8. Indices: 24, 25, 26
  // Set target block so neither Candidate 1 nor 2 are present (Overlap = 0)
  test_puzzle.candidates[24] = 0x0C; // Candidates 3, 4
  test_puzzle.candidates[25] = 0x0C;
  test_puzzle.candidates[26] = 0x0C;

  // Intersections of Cell 0 and Cell 12: Row 0 Col 3 (idx 3) and Row 1 Col 0 (idx 9)
  test_puzzle.candidates[3] = 0x07; // Candidates 1, 2, 3
  test_puzzle.candidates[9] = 0x0B; // Candidates 1, 2, 4

  TechniqueResult result = find_chute_remote_pairs(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  // Candidates 1 and 2 (0x03) should be eliminated
  TEST_ASSERT_EQUAL_HEX16(0x04, test_puzzle.candidates[3]);
  TEST_ASSERT_EQUAL_HEX16(0x08, test_puzzle.candidates[9]);
}

// -----------------------------------------------------------------------------
// Test 2: Horizontal Chute Remote Pair - 1 Overlap
// Expectation: Eliminates only the overlapping candidate from the intersections.
// -----------------------------------------------------------------------------
void test_horizontal_chute_one_overlap_eliminates_overlap(void) {
  test_puzzle.candidates[0] = 0x03;  // Candidates 1, 2
  test_puzzle.candidates[12] = 0x03; // Candidates 1, 2

  // Target block indices: 24, 25, 26
  // Set target block to contain Candidate 1 (0x01) but not Candidate 2 (0x02)
  test_puzzle.candidates[24] = 0x0D; // Candidates 1, 3, 4
  test_puzzle.candidates[25] = 0x0C;
  test_puzzle.candidates[26] = 0x0C;

  // Overlap mask should resolve to 0x01 (Candidate 1 only)
  test_puzzle.candidates[3] = 0x07; // Candidates 1, 2, 3

  TechniqueResult result = find_chute_remote_pairs(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  // Only candidate 1 (0x01) should be eliminated, candidate 2 remains
  TEST_ASSERT_EQUAL_HEX16(0x06, test_puzzle.candidates[3]);
}

// -----------------------------------------------------------------------------
// Test 3: Vertical Chute Remote Pair - 0 Overlap
// Expectation: Symmetrical execution of the vertical stack logic.
// -----------------------------------------------------------------------------
void test_vertical_chute_zero_overlap_eliminates_both(void) {
  // Cell 1: Row 0, Col 0 (Band 0, Stack 0). Mask: 0x03
  test_puzzle.candidates[0] = 0x03;

  // Cell 2: Row 3, Col 1 (Band 1, Stack 0). Mask: 0x03
  test_puzzle.candidates[28] = 0x03;

  // Target block: Band 2, Stack 0 -> Rows 6,7,8 and Cols 0,1,2
  // Unused Col: 2. Unused Band block: Rows 6, 7, 8. Indices: 56, 65, 74
  test_puzzle.candidates[56] = 0x0C;
  test_puzzle.candidates[65] = 0x0C;
  test_puzzle.candidates[74] = 0x0C;

  // Intersections of Cell 0 and Cell 28: Row 0 Col 1 (idx 1) and Row 3 Col 0 (idx 27)
  test_puzzle.candidates[1] = 0x07;
  test_puzzle.candidates[27] = 0x07;

  TechniqueResult result = find_chute_remote_pairs(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  TEST_ASSERT_EQUAL_HEX16(0x04, test_puzzle.candidates[1]);
  TEST_ASSERT_EQUAL_HEX16(0x04, test_puzzle.candidates[27]);
}

// -----------------------------------------------------------------------------
// Test 4: Negative Case - Target Block Has Both Candidates (Overlap == 2)
// Expectation: Fails to form a valid remote pair constraint, returns NO_PROGRESS.
// -----------------------------------------------------------------------------
void test_chute_remote_pair_fails_on_two_overlaps(void) {
  test_puzzle.candidates[0] = 0x03;
  test_puzzle.candidates[12] = 0x03;

  // Target block contains both Candidates 1 and 2
  test_puzzle.candidates[24] = 0x0F; // Candidates 1, 2, 3, 4
  test_puzzle.candidates[25] = 0x0C;
  test_puzzle.candidates[26] = 0x0C;

  test_puzzle.candidates[3] = 0x07;

  TechniqueResult result = find_chute_remote_pairs(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
  // Intersection cell should remain completely unchanged
  TEST_ASSERT_EQUAL_HEX16(0x07, test_puzzle.candidates[3]);
}

// -----------------------------------------------------------------------------
// Test 5: Edge Case - Valid Pair, but Intersections Have No Matches
// Expectation: Routine detects the pair but finds nothing to eliminate. 
// -----------------------------------------------------------------------------
void test_chute_remote_pair_no_intersections_to_eliminate(void) {
  test_puzzle.candidates[0] = 0x03;
  test_puzzle.candidates[12] = 0x03;

  test_puzzle.candidates[24] = 0x0C;
  test_puzzle.candidates[25] = 0x0C;
  test_puzzle.candidates[26] = 0x0C;

  // Intersections lack candidates 1 and 2
  test_puzzle.candidates[3] = 0x0C;
  test_puzzle.candidates[4] = 0x0C;
  test_puzzle.candidates[5] = 0x0C;
  test_puzzle.candidates[9] = 0x0C;
  test_puzzle.candidates[10] = 0x0C;
  test_puzzle.candidates[11] = 0x0C;

  TechniqueResult result = find_chute_remote_pairs(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 6: Boundary Case - Max Bivalue Bins
// Expectation: Processes correctly up to the declared limit of 18 pairs.
// -----------------------------------------------------------------------------
void test_chute_remote_pair_bivalue_array_boundary(void) {
  // Populate exactly 18 cells with the same bivalue mask to test bin limits
  for(int i = 0; i < 18; i++) {
    test_puzzle.candidates[i] = 0x03;
  }

  // Attempting to evaluate logic on this artificial board will yield NO_PROGRESS
  // because placing 18 identical bivalues in the first two bands breaks Sudoku 
  // rules and forces intersection overlaps > 1, but it proves the array does 
  // not segfault or fail indexing.
  TechniqueResult result = find_chute_remote_pairs(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_horizontal_chute_zero_overlap_eliminates_both);
  RUN_TEST(test_horizontal_chute_one_overlap_eliminates_overlap);
  RUN_TEST(test_vertical_chute_zero_overlap_eliminates_both);
  RUN_TEST(test_chute_remote_pair_fails_on_two_overlaps);
  RUN_TEST(test_chute_remote_pair_no_intersections_to_eliminate);
  RUN_TEST(test_chute_remote_pair_bivalue_array_boundary);
  return UNITY_END();
}
