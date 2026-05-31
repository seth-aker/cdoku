#include "unity.h"
#include <string.h>
#include "xy_wing.h"
#include "puzzle.h"
#include "utils.h"

static Puzzle test_puzzle;

// Forward declaration of the step applicator to track state changes in tests.
extern void apply_step(Puzzle* puzzle, Step step);

void setUp(void) {
  // Initialize a blank puzzle with all 9 candidates active in every cell.
  // Because the engine filters for `popcount == 2`, cells left at 0x01FF 
  // are safely ignored during the bivalue collection phase.
  for(int i = 0; i < 81; i++) {
    test_puzzle.cells[i] = 0;
    test_puzzle.candidates[i] = 0x01FF;
  }
}

void tearDown(void) {
  // Clean up if necessary
}

// -----------------------------------------------------------------------------
// Test 1: Valid XY-Wing (Standard Elimination)
// Expectation: A Pivot sees two non-peer Pincers. The Z candidate (shared by 
// the Pincers) is eliminated from all cells that can see BOTH Pincers.
// -----------------------------------------------------------------------------
void test_xy_wing_eliminates_z_candidate(void) {
  // Pivot: Row 0, Col 0 (Idx 0). Candidates: 1, 2 (Mask: 0x03)
  test_puzzle.candidates[0] = 0x03;

  // Pincer 1: Row 0, Col 4 (Idx 4). Candidates: 1, 3 (Mask: 0x05)
  // Sees Pivot via Row 0.
  test_puzzle.candidates[4] = 0x05;

  // Pincer 2: Row 1, Col 1 (Idx 10). Candidates: 2, 3 (Mask: 0x06)
  // Sees Pivot via Block 0.
  test_puzzle.candidates[10] = 0x06;

  // P1 and P2 do NOT see each other (Different Row, Col, and Block). Valid geometry.
  // The Z candidate is 3 (Mask: 0x04).

  // Target 1: Row 1, Col 4 (Idx 13). 
  // Sees P1 via Col 4. Sees P2 via Row 1.
  test_puzzle.candidates[13] = 0x01FF;

  // Target 2: Row 0, Col 1 (Idx 1). 
  // Sees P1 via Row 0. Sees P2 via Block 0.
  test_puzzle.candidates[1] = 0x01FF;

  TechniqueResult result = find_xy_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  // Candidate 3 (0x04) must be eliminated from the intersection targets
  TEST_ASSERT_EQUAL_HEX16(0x01FB, test_puzzle.candidates[13]);
  TEST_ASSERT_EQUAL_HEX16(0x01FB, test_puzzle.candidates[1]);

  // Pivot and Pincers must remain completely untouched
  TEST_ASSERT_EQUAL_HEX16(0x03, test_puzzle.candidates[0]);
  TEST_ASSERT_EQUAL_HEX16(0x05, test_puzzle.candidates[4]);
  TEST_ASSERT_EQUAL_HEX16(0x06, test_puzzle.candidates[10]);
}

// -----------------------------------------------------------------------------
// Test 2: False Positive (Naked Triple / Pincers are Peers)
// Expectation: If P1 and P2 see each other, it is a Naked Triple, not an 
// XY-Wing. The anti-peer check must catch this and bypass it.
// -----------------------------------------------------------------------------
void test_xy_wing_fails_if_pincers_are_peers(void) {
  // All three cells placed in Block 0
  test_puzzle.candidates[0] = 0x03; // Pivot: Row 0, Col 0 (1, 2)
  test_puzzle.candidates[1] = 0x05; // P1: Row 0, Col 1 (1, 3)
  test_puzzle.candidates[2] = 0x06; // P2: Row 0, Col 2 (2, 3)

  // Because P1 and P2 are in the same block/row, they are peers.

  TechniqueResult result = find_xy_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 3: False Positive (Ecosystem Not Closed)
// Expectation: The bitwise union of the three cells exceeds 3 active bits.
// The structure is invalid.
// -----------------------------------------------------------------------------
void test_xy_wing_fails_if_ecosystem_not_closed(void) {
  test_puzzle.candidates[0] = 0x03;  // Pivot: Row 0, Col 0 (Candidates: 1, 2)
  test_puzzle.candidates[4] = 0x05;  // P1: Row 0, Col 4 (Candidates: 1, 3)

  // Introduce Candidate 4 instead of 3. (Mask: 0x0A -> Candidates 2, 4)
  test_puzzle.candidates[10] = 0x0A; // P2: Row 1, Col 1

  // Bitwise union popcount is 4 (Candidates 1, 2, 3, 4). Not an XY-Wing.

  TechniqueResult result = find_xy_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 4: Edge Case (Geometry valid, but Z candidate already solved/missing)
// Expectation: The XY-Wing is mathematically present, but the kill zones 
// contain no actionable bits to flip.
// -----------------------------------------------------------------------------
void test_xy_wing_no_progress_if_target_lacks_z_candidate(void) {
  // Valid XY-Wing geometry
  test_puzzle.candidates[0] = 0x03;  // Pivot
  test_puzzle.candidates[4] = 0x05;  // P1
  test_puzzle.candidates[10] = 0x06; // P2

  // Target 1 (13) and Target 2 (1) have candidate 3 manually stripped out
  test_puzzle.candidates[13] = 0x01FB; // Missing 0x04
  test_puzzle.candidates[1] = 0x01FB;  // Missing 0x04

  // To ensure a clean test, strip candidate 3 from ALL other potential 
  // secondary intersections (e.g., R1C3, R0C2) that the engine might sweep.
  for(int i = 0; i < 81; i++) {
    if(i != 4 && i != 10) { // Don't strip it from the pincers
      test_puzzle.candidates[i] &= ~0x04;
    }
  }

  TechniqueResult result = find_xy_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 5: Negative Case (Pivot shares 2 candidates with Pincer)
// Expectation: The `popcount(peer & pivot) == 1` check correctly filters out 
// identical bivalue cells from acting as pincers.
// -----------------------------------------------------------------------------
void test_xy_wing_fails_if_pincer_identical_to_pivot(void) {
  test_puzzle.candidates[0] = 0x03; // Pivot: 1, 2
  test_puzzle.candidates[4] = 0x03; // P1: 1, 2 (Identical to Pivot)
  test_puzzle.candidates[10] = 0x06; // P2: 2, 3

  TechniqueResult result = find_xy_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_xy_wing_eliminates_z_candidate);
  RUN_TEST(test_xy_wing_fails_if_pincers_are_peers);
  RUN_TEST(test_xy_wing_fails_if_ecosystem_not_closed);
  RUN_TEST(test_xy_wing_no_progress_if_target_lacks_z_candidate);
  RUN_TEST(test_xy_wing_fails_if_pincer_identical_to_pivot);
  return UNITY_END();
}
