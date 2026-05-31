#include "unity.h"
#include <string.h>
#include "w_wing.h"
#include "puzzle.h"
#include "utils.h"

static Puzzle test_puzzle;


void setUp(void) {
  // Initialize a blank puzzle with all 9 candidates active in every cell
  for(int i = 0; i < 81; i++) {
    test_puzzle.cells[i] = 0;
    test_puzzle.candidates[i] = 0x01FF;
  }
}

void tearDown(void) {
  // Clean up if necessary
}

// -----------------------------------------------------------------------------
// Test 1: Valid W-Wing (Pass 1)
// Expectation: Candidate A is the strong link, Candidate B is eliminated.
// -----------------------------------------------------------------------------
void test_w_wing_candidate_a_strong_link_eliminates_b(void) {
  // Bivalue pair: Candidates 1 & 2 (Mask: 0x03)
  test_puzzle.candidates[0] = 0x03;  // BV1: Row 0, Col 0
  test_puzzle.candidates[80] = 0x03; // BV2: Row 8, Col 8

  // Forge a strong link for Candidate 1 in Column 4.
  // SL Nodes: Row 0, Col 4 (Idx 4) and Row 8, Col 4 (Idx 76)
  // We must strip Candidate 1 from all other cells in Column 4.
  for(int r = 1; r < 8; r++) {
    test_puzzle.candidates[r * 9 + 4] &= ~0x01;
  }

  // Geometry check: 
  // - BV1 (0) sees SL Node 1 (4) via Row 0.
  // - BV2 (80) sees SL Node 2 (76) via Row 8.
  // - Intersections of BV1 and BV2 are R0C8 (Idx 8) and R8C0 (Idx 72).

  TechniqueResult result = find_w_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  // Candidate 2 (0x02) should be eliminated from both intersections
  TEST_ASSERT_EQUAL_HEX16(0x01FD, test_puzzle.candidates[8]);
  TEST_ASSERT_EQUAL_HEX16(0x01FD, test_puzzle.candidates[72]);
  // Bivalue nodes should remain untouched
  TEST_ASSERT_EQUAL_HEX16(0x03, test_puzzle.candidates[0]);
}

// -----------------------------------------------------------------------------
// Test 2: Valid W-Wing (Pass 2)
// Expectation: Candidate B is the strong link, Candidate A is eliminated.
// -----------------------------------------------------------------------------
void test_w_wing_candidate_b_strong_link_eliminates_a(void) {
  // Bivalue pair: Candidates 1 & 2 (Mask: 0x03)
  test_puzzle.candidates[10] = 0x03; // BV1: Row 1, Col 1
  test_puzzle.candidates[70] = 0x03; // BV2: Row 7, Col 7

  // Forge a strong link for Candidate 2 in Column 4.
  // SL Nodes: Row 1, Col 4 (Idx 13) and Row 7, Col 4 (Idx 67)
  for(int r = 0; r < 9; r++) {
    if(r != 1 && r != 7) {
      test_puzzle.candidates[r * 9 + 4] &= ~0x02;
    }
  }

  // Intersections of BV1 (10) and BV2 (70) are R1C7 (Idx 16) and R7C1 (Idx 64).

  TechniqueResult result = find_w_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  // Candidate 1 (0x01) should be eliminated from both intersections
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[16]);
  TEST_ASSERT_EQUAL_HEX16(0x01FE, test_puzzle.candidates[64]);
}

// -----------------------------------------------------------------------------
// Test 3: Invalid Geometry (Bivalues are Peers)
// Expectation: W-Wings explicitly require the bivalue cells to NOT be peers.
// -----------------------------------------------------------------------------
void test_w_wing_fails_when_bivalues_are_peers(void) {
  test_puzzle.candidates[0] = 0x03; // BV1: Row 0, Col 0
  test_puzzle.candidates[2] = 0x03; // BV2: Row 0, Col 2 (Peer of BV1)

  // Forge a valid strong link anyway to ensure the peer-check is the constraint
  // SL Nodes: Row 1, Col 0 (Idx 9) and Row 1, Col 2 (Idx 11)
  for(int r = 0; r < 9; r++) {
    if(r != 1) {
      test_puzzle.candidates[r * 9 + 0] &= ~0x01;
      test_puzzle.candidates[r * 9 + 2] &= ~0x01;
    }
  }

  TechniqueResult result = find_w_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 4: Invalid Link (Weak Link Instead of Strong Link)
// Expectation: Fails to form if the connecting candidates have >2 positions.
// -----------------------------------------------------------------------------
void test_w_wing_fails_without_strict_strong_link(void) {
  test_puzzle.candidates[0] = 0x03;
  test_puzzle.candidates[80] = 0x03;

  // We leave Col 4 completely full (0x01FF in all 9 cells).
  // Because Candidate 1 appears 9 times in Col 4, no strong link exists.
  // The algorithm must bypass it.

  TechniqueResult result = find_w_wings(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 5: Edge Case (Intersection already solved/missing candidate)
// Expectation: Logic is sound, but no mutable bits remain to be flipped.
// -----------------------------------------------------------------------------
void test_w_wing_no_progress_if_intersections_empty(void) {
  test_puzzle.candidates[0] = 0x03;
  test_puzzle.candidates[80] = 0x03;

  // Forge the strong link (Same as Test 1)
  for(int r = 1; r < 8; r++) {
    test_puzzle.candidates[r * 9 + 4] &= ~0x01;
  }

  // Manually strip Candidate 2 from the kill zones beforehand
  test_puzzle.candidates[8] &= ~0x02;
  test_puzzle.candidates[72] &= ~0x02;

  TechniqueResult result = find_w_wings(&test_puzzle);

  // The W-Wing exists mathematically, but the engine correctly identifies
  // that no state mutations are required.
  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_w_wing_candidate_a_strong_link_eliminates_b);
  RUN_TEST(test_w_wing_candidate_b_strong_link_eliminates_a);
  RUN_TEST(test_w_wing_fails_when_bivalues_are_peers);
  RUN_TEST(test_w_wing_fails_without_strict_strong_link);
  RUN_TEST(test_w_wing_no_progress_if_intersections_empty);
  return UNITY_END();
}
