#include <stdbool.h>
#include "skyscraper.h"

TechniqueResult find_skyscrapers(Puzzle* puzzle) {
  bool progress_made = false;
  SkyscraperContext search_context;
  for(int candidate = 1; candidate <= PUZZLE_WIDTH; ++candidate) {
    search_context.base_house_count = 0;
    uint16_t candidate_mask = 1 << (candidate - 1);
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      const uint8_t* row_idxs = ROW_TO_IDXS[i];
      int positions[2];
      SearchParams params = {
        .mask = candidate_mask,
        .max_count = 2
      };
      int cand_count = get_candidate_positions(puzzle, row_idxs, params, positions);
      if(cand_count == 2) {
        int base_idx = search_context.base_house_count;
        search_context.base_house_idxs[base_idx] = i;
        search_context.cover_houses[base_idx][0] = positions[0];
        search_context.cover_houses[base_idx][1] = positions[1];
        search_context.base_house_count++;
      }
    }

    for(int r1 = 0; r1 < search_context.base_house_count - 1; ++r1) {
      const int* cover_col_1 = search_context.cover_houses[r1];
      for(int r2 = r1 + 1; r2 < search_context.base_house_count; ++r2) {
        const int* cover_col_2 = search_context.cover_houses[r2];

        bool left_match = (cover_col_1[0] == cover_col_2[0]);
        bool right_match = (cover_col_1[1] == cover_col_2[1]);
        // if both are true or both are false, not a skyscraper
        if(left_match == right_match) {
          continue;
        }
        int top_1_col = left_match ? cover_col_1[1] : cover_col_1[0];
        int top_2_col = left_match ? cover_col_2[1] : cover_col_2[0];

        uint8_t top_1_idx = ROW_TO_IDXS[r1][top_1_col];
        uint8_t top_2_idx = ROW_TO_IDXS[r2][top_2_col];

        uint8_t* top_1_peers = CELL_PEERS_LOOKUP[top_1_idx];
        uint8_t* top_2_peers = CELL_PEERS_LOOKUP[top_2_idx];


      }
    }
  }
}
