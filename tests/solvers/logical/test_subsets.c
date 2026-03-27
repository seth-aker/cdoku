#include "unity.h"
#include "subsets.h" 
#include <string.h>

// Global test state
Puzzle p;
House h;

// Helper to manually set a candidate mask in both the House snapshot and main Puzzle
void set_candidates(int index, uint16_t mask) {
    p.candidates[index] = mask;
    h.candidates[index] = mask;
}


void setUp(void) {
    memset(p.cells, 0, sizeof(p.cells));
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        p.candidates[i] = 0x1FF; // 9 bits set (1-9 available)
    }
    
    for (int i = 0; i < PUZZLE_WIDTH; ++i) {
        h.cells[i] = 0;
        h.candidates[i] = 0x1FF;
        h.idx_lookup[i] = i; // Map house index 0-8 directly to puzzle index 0-8
    }
}


void tearDown(void) {

}


void test_find_naked_pair_actionable(void) {
    // Set up a Naked Pair of [3, 7] in index 0 and 1
    uint16_t pair_mask = (1 << 2) | (1 << 6); 
    set_candidates(0, pair_mask);
    set_candidates(1, pair_mask);
    
    // Give index 2 some overlapping candidates [3, 7, 8]
    set_candidates(2, pair_mask | (1 << 7));

    TEST_ASSERT_EQUAL(PROGRESS_MADE, find_naked_pair(&p, &h));
    
    // Assuming apply_step works, the 3 and 7 should be erased from puzzle index 2
    TEST_ASSERT_EQUAL_HEX16((1 << 7), p.candidates[2]); 
}

void test_find_naked_pair_non_actionable_stagnates(void) {
    uint16_t pair_mask = (1 << 2) | (1 << 6); 
    set_candidates(0, pair_mask);
    set_candidates(1, pair_mask);
    
    // Give the rest of the house completely unrelated candidates [1, 2]
    for (int i = 2; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 0) | (1 << 1) | (1 << 7)); 
    }

    TEST_ASSERT_EQUAL(NO_PROGRESS, find_naked_pair(&p, &h));
}

void test_find_naked_triple_actionable(void) {
    // Create a [1,2], [2,3], [1,3] triple (Union is [1, 2, 3])
    set_candidates(0, (1 << 0) | (1 << 1));
    set_candidates(1, (1 << 1) | (1 << 2));
    set_candidates(2, (1 << 0) | (1 << 2));
    
    // Overlapping cell with [1, 2, 3, 9]
    set_candidates(3, (1 << 0) | (1 << 1) | (1 << 2) | (1 << 8));

    TEST_ASSERT_EQUAL(PROGRESS_MADE, find_naked_triple(&p, &h));
    TEST_ASSERT_EQUAL_HEX16((1 << 8), p.candidates[3]); // Only the 9 should remain
}

void test_find_naked_triple_non_actionable_stagnates(void) {
    set_candidates(0, (1 << 0) | (1 << 1));
    set_candidates(1, (1 << 1) | (1 << 2));
    set_candidates(2, (1 << 0) | (1 << 2));
    for (int i = 3; i < PUZZLE_WIDTH; ++i) {
      set_candidates(i, (1 << 5) | (1 << 6) | (1 << 7)); 
    }
    TEST_ASSERT_EQUAL(NO_PROGRESS, find_naked_pair(&p, &h));
}
void test_find_naked_quad_actionable(void) {
    // Create a strict Quad of [1, 2, 3, 4] across 4 cells
    uint16_t quad_mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    set_candidates(0, quad_mask);
    set_candidates(1, quad_mask);
    set_candidates(2, quad_mask);
    set_candidates(3, quad_mask);
    
    // Overlapping cell with [1, 2, 3, 4, 5]
    set_candidates(4, quad_mask | (1 << 4));

    TEST_ASSERT_EQUAL(PROGRESS_MADE, find_naked_quad(&p, &h));
    TEST_ASSERT_EQUAL_HEX16((1 << 4), p.candidates[4]); // Only the 5 should remain
}

void test_find_naked_quad_non_actionable_stagnates(void) {
  uint16_t quad_mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    set_candidates(0, quad_mask);
    set_candidates(1, quad_mask);
    set_candidates(2, quad_mask);
    set_candidates(3, quad_mask);

  for(int i = 4; i < PUZZLE_WIDTH; ++i) {
    set_candidates(i, (1 << 5));
  }
  TEST_ASSERT_EQUAL(NO_PROGRESS, find_naked_quad(&p, &h));
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_find_naked_pair_actionable);
    RUN_TEST(test_find_naked_pair_non_actionable_stagnates);
    RUN_TEST(test_find_naked_triple_actionable);
    RUN_TEST(test_find_naked_triple_non_actionable_stagnates);
    
    RUN_TEST(test_find_naked_quad_actionable);
    RUN_TEST(test_find_naked_quad_non_actionable_stagnates);
    return UNITY_END();
}