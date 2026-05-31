#include "unity.h"
#include <string.h>
#include "xyz_wing.h"
#include "puzzle.h"
#include "utils.h"

static Puzzle test_puzzle;

// Forward declaration of the step applicator to track state changes in tests.
extern void apply_step(Puzzle* puzzle, Step step);

void setUp(void) {
  // Initialize a blank puzzle. Cells with 9 candidates (0x01FF) will be safely 
  // ignored by the popcount == 2 and popcount == 3 filters.
  for(int i = 0; i < 81; i++) {
    test_puzzle.cells[i] = 0;
    test_puzzle.candidates[i] = 0x01FF;
  }
}

void tearDown(void) {
  // Clean up if necessary
}

// -----------------------------------------------------------------------------
// Test 1: Valid XYZ-Wing (Standard Elimination)
// Expectation: Pivot (3 candidates) sees two non-peer Pincers (2 candidates). 
// The Z candidate is eliminated from cells that see ALL THREE nodes.
// -----------------------------------------------------------------------------
void test_xyz_wing_eliminates_z_candidate_from_triple_intersection(void) {
  // Pivot: Row 0, Col 0 (Idx 0). Candidates: 1, 2, 3 (Mask: 0x07)
  test_puzzle.candidates[0] = 0x07;

  // Pincer 1: Row 0, Col 4 (Idx 4). Candidates: 1, 3 (Mask: 0x05)
  // Sees Pivot via Row 0.
  test_puzzle.candidates[4] = 0x05;

  // Pincer 2: Row 1, Col 1 (Idx 10). Candidates: 2, 3 (Mask: 0x06)
  // Sees Pivot via Block 0.
  test_puzzle.candidates[10] = 0x06;

  // Valid Geometry: P1 and P2 do NOT see each other.
  // Z Candidate: 3 (Mask: 0x04)

  // Target 1: Row 0, Col 1 (Idx 1). 
  // Sees Pivot (Row/Blk), Sees P1 (Row), Sees P2 (Col/Blk). (KILL ZONE)
  test_puzzle.candidates[1] = 0x01FF;

  // Target 2: Row 0, Col 2 (Idx 2).
  // Sees Pivot (Row/Blk), Sees P1 (Row), Sees P2 (Blk). (KILL ZONE)
  test_puzzle.candidates[2] = 0x01FF;

  // Negative Control: Row 1, Col 0 (Idx 9).
  // Sees Pivot (Col/Blk), Sees P2 (Row/Blk), but DOES NOT see P1 (R0C4).
  test_puzzle.candidates[9] = 0x01FF;

  TechniqueResult result = find_xyz_wing(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);

  // Candidate 3 (0x04) must be eliminated from valid intersections
  TEST_ASSERT_EQUAL_HEX16(0x01FB, test_puzzle.candidates[1]);
  TEST_ASSERT_EQUAL_HEX16(0x01FB, test_puzzle.candidates[2]);

  // The negative control must remain untouched because it doesn't see P1
  TEST_ASSERT_EQUAL_HEX16(0x01FF, test_puzzle.candidates[9]);

  // Pivot and Pincers must remain completely untouched
  TEST_ASSERT_EQUAL_HEX16(0x07, test_puzzle.candidates[0]);
  TEST_ASSERT_EQUAL_HEX16(0x05, test_puzzle.candidates[4]);
  TEST_ASSERT_EQUAL_HEX16(0x06, test_puzzle.candidates[10]);
}

// -----------------------------------------------------------------------------
// Test 2: False Positive (Pincers are Peers -> Naked Triple)
// Expectation: If P1 and P2 see each other, the structure is a Naked Triple.
// The XYZ-Wing engine must detect the peer collision and bypass it.
// -----------------------------------------------------------------------------
void test_xyz_wing_fails_if_pincers_are_peers(void) {
  test_puzzle.candidates[0] = 0x07; // Pivot: Row 0, Col 0 (1, 2, 3)
  test_puzzle.candidates[1] = 0x05; // P1: Row 0, Col 1 (1, 3)
  test_puzzle.candidates[2] = 0x06; // P2: Row 0, Col 2 (2, 3)

  // P1 and P2 share Row 0 and Block 0. They are peers.

  TechniqueResult result = find_xyz_wing(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 3: False Positive (Pincers Do Not Share Exactly 1 Candidate)
// Expectation: If P1 and P2 do not share exactly one candidate, the `popcount`
// intersection filter must reject the structure.
// -----------------------------------------------------------------------------
void test_xyz_wing_fails_if_pincers_share_zero_candidates(void) {
  test_puzzle.candidates[0] = 0x07;  // Pivot: 1, 2, 3
  test_puzzle.candidates[4] = 0x05;  // P1: 1, 3

  // Introduce Candidate 2 and 4 instead of 2 and 3. 
  // P2 mask 0x0A (Candidates 2, 4). Shares 0 candidates with P1.
  test_puzzle.candidates[10] = 0x0A; // P2: Row 1, Col 1

  TechniqueResult result = find_xyz_wing(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 4: Edge Case (Geometry valid, but Z candidate already solved/missing)
// Expectation: The XYZ-Wing exists mathematically, but the intersecting kill 
// zones contain no actionable bits to flip.
// -----------------------------------------------------------------------------
void test_xyz_wing_no_progress_if_target_lacks_z_candidate(void) {
  test_puzzle.candidates[0] = 0x07;  // Pivot
  test_puzzle.candidates[4] = 0x05;  // P1
  test_puzzle.candidates[10] = 0x06; // P2

  // Targets (Idx 1 and 2) have Candidate 3 manually stripped out
  test_puzzle.candidates[1] = 0x01FB; // Missing 0x04
  test_puzzle.candidates[2] = 0x01FB; // Missing 0x04

  // Strip candidate 3 from all other cells to ensure a clean sweep
  for(int i = 0; i < 81; i++) {
    if(i != 0 && i != 4 && i != 10) {
      test_puzzle.candidates[i] &= ~0x04;
    }
  }

  TechniqueResult result = find_xyz_wing(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_xyz_wing_eliminates_z_candidate_from_triple_intersection);
  RUN_TEST(test_xyz_wing_fails_if_pincers_are_peers);
  RUN_TEST(test_xyz_wing_fails_if_pincers_share_zero_candidates);
  RUN_TEST(test_xyz_wing_no_progress_if_target_lacks_z_candidate);
  return UNITY_END();
}
