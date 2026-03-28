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
    for(int i = 0; i < TOTAL_CELLS; ++i) {
        p.candidates[i] = 0x1FF; // 9 bits set (1-9 available)
    }

    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
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
    for(int i = 2; i < PUZZLE_WIDTH; ++i) {
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
    for(int i = 3; i < PUZZLE_WIDTH; ++i) {
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
void test_find_hidden_pair_actionable(void) {
    // Target Pair: [1, 2] -> Bits 0 and 1 -> 0x0003
    // We will hide these in cells 0 and 1.

    // Cell 0: Has [1, 2] + Noise [8] -> 0x0083
    set_candidates(0, (1 << 0) | (1 << 1) | (1 << 7));
    // Cell 1: Has [1, 2] + Noise [9] -> 0x0103
    set_candidates(1, (1 << 0) | (1 << 1) | (1 << 8));

    // Cells 2-8: Must contain random noise, but absolutely NO [1] or [2]
    // We use candidates 3 through 7 -> 0x007C
    for(int i = 2; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 2) | (1 << 3) | (1 << 4));
    }

    TEST_ASSERT_EQUAL(PROGRESS_MADE, find_hidden_pair(&p, &h));

    // The technique should strip the noise (8 and 9) from cells 0 and 1, leaving only 1 and 2
    TEST_ASSERT_EQUAL_HEX16(0x0003, p.candidates[0]);
    TEST_ASSERT_EQUAL_HEX16(0x0003, p.candidates[1]);
}

void test_find_hidden_pair_stagnates(void) {
    // If a pair is hidden, but the cells have NO noise, it is basically just a Naked Pair.
    // The function should recognize it but return NO_PROGRESS because no candidates are deleted.

    set_candidates(0, (1 << 0) | (1 << 1)); // Exactly [1, 2], no noise
    set_candidates(1, (1 << 0) | (1 << 1)); // Exactly [1, 2], no noise

    for(int i = 2; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 2) | (1 << 3)); // No 1s or 2s here
    }

    TEST_ASSERT_EQUAL(NO_PROGRESS, find_hidden_pair(&p, &h));
}

void test_find_hidden_triple_actionable(void) {
    // Target Triple: [1, 2, 3] -> Bits 0, 1, 2 -> 0x0007
    // Hiding in cells 0, 1, 2

    set_candidates(0, 0x0007 | (1 << 5)); // Triple + Noise [6]
    set_candidates(1, 0x0007 | (1 << 6)); // Triple + Noise [7]
    set_candidates(2, 0x0007 | (1 << 7)); // Triple + Noise [8]

    // Cells 3-8: Fill with Candidates 4, 5, 9 (No 1, 2, 3)
    for(int i = 3; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 3) | (1 << 4) | (1 << 8));
    }

    TEST_ASSERT_EQUAL(PROGRESS_MADE, find_hidden_triple(&p, &h));

    // All noise should be stripped
    TEST_ASSERT_EQUAL_HEX16(0x0007, p.candidates[0]);
    TEST_ASSERT_EQUAL_HEX16(0x0007, p.candidates[1]);
    TEST_ASSERT_EQUAL_HEX16(0x0007, p.candidates[2]);
}

void test_find_hidden_quad_actionable(void) {
    // Target Quad: [1, 2, 3, 4] -> Bits 0, 1, 2, 3 -> 0x000F
    // Hiding in cells 0, 1, 2, 3

    set_candidates(0, 0x000F | (1 << 8)); // Quad + Noise [9]
    set_candidates(1, 0x000F | (1 << 7)); // Quad + Noise [8]
    set_candidates(2, 0x000F | (1 << 6)); // Quad + Noise [7]
    set_candidates(3, 0x000F | (1 << 5)); // Quad + Noise [6]

    // Cells 4-8: Fill with Candidates 5, 6, 7 (No 1, 2, 3, 4)
    for(int i = 4; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 4) | (1 << 5) | (1 << 6));
    }

    TEST_ASSERT_EQUAL(PROGRESS_MADE, find_hidden_quad(&p, &h));

    // All noise should be stripped
    TEST_ASSERT_EQUAL_HEX16(0x000F, p.candidates[0]);
    TEST_ASSERT_EQUAL_HEX16(0x000F, p.candidates[1]);
    TEST_ASSERT_EQUAL_HEX16(0x000F, p.candidates[2]);
    TEST_ASSERT_EQUAL_HEX16(0x000F, p.candidates[3]);
}
void test_find_hidden_triple_stagnates(void) {
    // Hidden triple exists, but has absolutely no noise to strip.
    set_candidates(0, (1 << 0) | (1 << 1) | (1 << 2)); // [1, 2, 3]
    set_candidates(1, (1 << 0) | (1 << 1) | (1 << 2)); // [1, 2, 3]
    set_candidates(2, (1 << 0) | (1 << 1) | (1 << 2)); // [1, 2, 3]

    // Fill the rest with non-overlapping candidates
    for(int i = 3; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 3) | (1 << 4));
    }

    TEST_ASSERT_EQUAL(NO_PROGRESS, find_hidden_triple(&p, &h));
}

void test_find_hidden_quad_stagnates(void) {
    // Hidden quad exists, but has absolutely no noise to strip.
    uint16_t quad_mask = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    set_candidates(0, quad_mask); // [1, 2, 3, 4]
    set_candidates(1, quad_mask); // [1, 2, 3, 4]
    set_candidates(2, quad_mask); // [1, 2, 3, 4]
    set_candidates(3, quad_mask); // [1, 2, 3, 4]

    // Fill the rest with non-overlapping candidates
    for(int i = 4; i < PUZZLE_WIDTH; ++i) {
        set_candidates(i, (1 << 4) | (1 << 5));
    }

    TEST_ASSERT_EQUAL(NO_PROGRESS, find_hidden_quad(&p, &h));
}

void test_find_hidden_subsets_stagnate_on_insufficient_empty_cells(void) {
    // Test the early-exit logic when a house is mostly filled with numbers.
    // We will simulate a house with only 2 empty cells.

    for(int i = 0; i < 7; ++i) {
        h.cells[i] = i + 1;       // Mark the cell as solved
        h.candidates[i] = 0;      // Solved cells have 0 candidates
    }

    // The last two cells are empty
    set_candidates(7, (1 << 7) | (1 << 8));
    set_candidates(8, (1 << 7) | (1 << 8));

    // With only 2 empty cells:
    // Pair should run (but stagnate because there's no noise to remove)
    TEST_ASSERT_EQUAL(NO_PROGRESS, find_hidden_pair(&p, &h));

    // Triple and Quad should instantly abort and return NO_PROGRESS
    TEST_ASSERT_EQUAL(NO_PROGRESS, find_hidden_triple(&p, &h));
    TEST_ASSERT_EQUAL(NO_PROGRESS, find_hidden_quad(&p, &h));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_find_naked_pair_actionable);
    RUN_TEST(test_find_naked_pair_non_actionable_stagnates);
    RUN_TEST(test_find_naked_triple_actionable);
    RUN_TEST(test_find_naked_triple_non_actionable_stagnates);

    RUN_TEST(test_find_naked_quad_actionable);
    RUN_TEST(test_find_naked_quad_non_actionable_stagnates);

    RUN_TEST(test_find_hidden_pair_actionable);
    RUN_TEST(test_find_hidden_pair_stagnates);
    RUN_TEST(test_find_hidden_triple_actionable);
    RUN_TEST(test_find_hidden_triple_stagnates);
    RUN_TEST(test_find_hidden_quad_actionable);
    RUN_TEST(test_find_hidden_quad_stagnates);

    return UNITY_END();
}
