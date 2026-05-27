#include "unity.h"
#include <string.h>
#include "simple_colors.h"
#include "puzzle.h"
#include "utils.h"

static Puzzle test_puzzle;

// Assumes apply_step is linked and mutates test_puzzle state.
extern void apply_step(Puzzle* puzzle, Step step);

void setUp(void) {
  // Initialize a blank puzzle with 0 candidates to prevent ANY 
  // accidental strong links from forming.
  for(int i = 0; i < 81; i++) {
    test_puzzle.cells[i] = 0;
    test_puzzle.candidates[i] = 0;
  }
}

void tearDown(void) {
}

// -----------------------------------------------------------------------------
// Test 1: Color Trap (Rule 2)
// -----------------------------------------------------------------------------
void test_simple_colors_rule_2_trap_eliminates_candidate(void) {
  uint16_t mask = 0x0001; // Candidate 1

  // Build a 4-node chain:
  // 0 (R0C0)  <--Row 0-->  8 (R0C8)
  // 72 (R8C0) <--Row 8--> 80 (R8C8)
  // Strong links in Col 0 (0, 72) and Col 8 (8, 80).
  test_puzzle.candidates[0] = mask;
  test_puzzle.candidates[8] = mask;
  test_puzzle.candidates[72] = mask;
  test_puzzle.candidates[80] = mask;

  // Target Cell: 36 (R4C0).
  // Shares Col 0 with 0 and 72. 
  // By placing it in Col 0, Col 0 now has 3 candidates (0, 36, 72).
  // This breaks the strong link in Col 0, making the chain an open line:
  // 72 (Color 0) - 80 (Color 1) - 8 (Color 0) - 0 (Color 1).
  // Target 36 sees 72 (Color 0) and 0 (Color 1). Trap triggers.

  // Give target an extra candidate (0x02) so it isn't completely empty after elimination
  test_puzzle.candidates[36] = mask | 0x0002;

  TechniqueResult result = find_simple_color(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);

  // Candidate 1 should be eliminated from the target cell
  TEST_ASSERT_EQUAL_HEX16(0x0002, test_puzzle.candidates[36]);
  // Chain nodes remain unaffected
  TEST_ASSERT_EQUAL_HEX16(mask, test_puzzle.candidates[0]);
}

// -----------------------------------------------------------------------------
// Test 2: Color Wrap (Rule 1)
// -----------------------------------------------------------------------------
void test_simple_colors_rule_1_wrap_places_values(void) {
  uint16_t mask = 0x0001; // Candidate 1

  // Build a 5-node cycle (requires an odd number of edges to force a wrap)
  // Node A: 0 (R0C0) 
  // Node B: 8 (R0C8)  -> Row 0
  // Node C: 80 (R8C8) -> Col 8
  // Node D: 73 (R8C1) -> Row 8
  // Node E: 19 (R2C1) -> Col 1
  // Loop E to A: 19 and 0 share Block 0.

  test_puzzle.candidates[0] = mask;
  test_puzzle.candidates[8] = mask;
  test_puzzle.candidates[80] = mask;
  test_puzzle.candidates[73] = mask;
  test_puzzle.candidates[19] = mask;

  // BFS Traversal assigns Color 0 to {0, 80, 73} and Color 1 to {8, 19}.
  // 80 and 73 are both Color 0 and share Row 8.
  // Color 0 is invalidated. Nodes with Color 1 (8 and 19) become the solution.

  TechniqueResult result = find_simple_color(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);
  // Assuming apply_step places the value in the puzzle->cells array
  TEST_ASSERT_EQUAL_UINT8(1, test_puzzle.cells[8]);
  TEST_ASSERT_EQUAL_UINT8(1, test_puzzle.cells[19]);
}

// -----------------------------------------------------------------------------
// Test 3: No Progress
// -----------------------------------------------------------------------------
void test_simple_colors_no_progress_on_valid_chain(void) {
  uint16_t mask = 0x0001;

  // Closed 4-node chain (Even length = perfectly valid bipartite graph)
  // 12 (R1C3) - 17 (R1C8) - 71 (R7C8) - 66 (R7C3) - 12
  test_puzzle.candidates[12] = mask;
  test_puzzle.candidates[17] = mask;
  test_puzzle.candidates[71] = mask;
  test_puzzle.candidates[66] = mask;

  TechniqueResult result = find_simple_color(&test_puzzle);

  TEST_ASSERT_EQUAL(NO_PROGRESS, result);
}

// -----------------------------------------------------------------------------
// Test 4: Disjoint Chains
// -----------------------------------------------------------------------------
void test_simple_colors_handles_disjoint_chains_independently(void) {
  uint16_t mask = 0x0001;

  // Chain 1: Valid closed 4-node chain (Will not yield progress)
  test_puzzle.candidates[12] = mask;
  test_puzzle.candidates[17] = mask;
  test_puzzle.candidates[71] = mask;
  test_puzzle.candidates[66] = mask;

  // Chain 2: Open 4-node chain trapping a target cell
  test_puzzle.candidates[0] = mask;
  test_puzzle.candidates[8] = mask;
  test_puzzle.candidates[72] = mask;
  test_puzzle.candidates[80] = mask;

  // Target for Chain 2: 36 (R4C0). Breaks Col 0 strong link for Chain 2.
  test_puzzle.candidates[36] = mask | 0x0002;

  TechniqueResult result = find_simple_color(&test_puzzle);

  TEST_ASSERT_EQUAL(PROGRESS_MADE, result);

  // Ensure the solver successfully bypassed Chain 1 and solved Chain 2
  TEST_ASSERT_EQUAL_HEX16(0x0002, test_puzzle.candidates[36]);
  // Chain 1 nodes should remain entirely unaffected
  TEST_ASSERT_EQUAL_HEX16(mask, test_puzzle.candidates[12]);
}

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_simple_colors_rule_2_trap_eliminates_candidate);
  RUN_TEST(test_simple_colors_rule_1_wrap_places_values);
  RUN_TEST(test_simple_colors_no_progress_on_valid_chain);
  RUN_TEST(test_simple_colors_handles_disjoint_chains_independently);
  return UNITY_END();
}
