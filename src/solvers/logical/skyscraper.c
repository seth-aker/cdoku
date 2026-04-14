#include <stdbool.h>
#include "skyscraper.h"
#include "utils.h"

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
      int* cover_col_1 = search_context.cover_houses[r1];
      for(int r2 = r1 + 1; r2 < search_context.base_house_count; ++r2) {
        int* cover_col_2 = search_context.cover_houses[r2];

        int shared_col, upper_col, lower_col = -1;
        if(cover_col_1[0] == cover_col_2[0]) {
          shared_col = cover_col_1[0];
          upper_col = cover_col_1[1];
          lower_col = cover_col_2[1];
        } else if(cover_col_1[1] == cover_col_2[0]) {
          shared_col = cover_col_1[1];
          upper_col = cover_col_1[0];
          lower_col = cover_col_2[1];
        } else if(cover_col_1[1] == cover_col_2[1]) {
          shared_col = cover_col_1[1];
          upper_col = cover_col_1[0];
          lower_col = cover_col_2[0];
        } else if(cover_col_1[0] == cover_col_2[1]) {
          shared_col = cover_col_1[0];
          upper_col = cover_col_1[1];
          lower_col = cover_col_2[0];
        } else {
          continue;
        }

        if(upper_col == lower_col) {
          continue;
        }

        int upper_row = search_context.base_house_idxs[r1];
        int lower_row = search_context.base_house_idxs[r2];

        uint8_t upper_idx = ROW_TO_IDXS[upper_row][upper_col];
        uint8_t lower_idx = ROW_TO_IDXS[lower_row][lower_col];

        const uint8_t* upper_peers = CELL_PEERS_LOOKUP[upper_idx];
        const uint8_t* lower_peers = CELL_PEERS_LOOKUP[lower_idx];

        uint8_t intersections[20];
        int intersections_count = get_uint8_intersection(upper_peers, lower_peers, 20, 20, intersections);

        for(int i = 0; i < intersections_count; ++i) {
          if(intersections[i] == upper_idx || intersections[i] == lower_idx) {
            continue;
          }
          uint16_t removed = puzzle->candidates[intersections[i]] & candidate_mask;
          if(removed) {
            Step step = {
              .eliminated_mask = removed,
              .placed_val = 0,
              .target_cell = intersections[i],
              .technique = SKYSCRAPER
            };
            apply_step(puzzle, step);
            progress_made = true;
          }
        }
        if(progress_made) {
          return PROGRESS_MADE;
        }
      }
    }

    // 
    search_context.base_house_count = 0;
    for(int i = 0; i < PUZZLE_WIDTH; ++i) {
      const uint8_t* col_idxs = COL_TO_IDXS[i];
      int positions[2];
      SearchParams params = {
        .mask = candidate_mask,
        .max_count = 2
      };
      int cand_count = get_candidate_positions(puzzle, col_idxs, params, positions);
      if(cand_count == 2) {
        int base_idx = search_context.base_house_count;
        search_context.base_house_idxs[base_idx] = i;
        search_context.cover_houses[base_idx][0] = positions[0];
        search_context.cover_houses[base_idx][1] = positions[1];
        search_context.base_house_count++;
      }
    }

    for(int c1 = 0; c1 < search_context.base_house_count - 1; ++c1) {
      int* cover_row_1 = search_context.cover_houses[c1];
      for(int c2 = c1 + 1; c2 < search_context.base_house_count; ++c2) {
        int* cover_row_2 = search_context.cover_houses[c2];

        int shared_row = -1, left_row = -1, right_row = -1;
        if(cover_row_1[0] == cover_row_2[0]) {
          shared_row = cover_row_1[0];
          left_row = cover_row_1[1];
          right_row = cover_row_2[1];
        } else if(cover_row_1[1] == cover_row_2[0]) {
          shared_row = cover_row_1[1];
          left_row = cover_row_1[0];
          right_row = cover_row_2[1];
        } else if(cover_row_1[1] == cover_row_2[1]) {
          shared_row = cover_row_1[1];
          left_row = cover_row_1[0];
          right_row = cover_row_2[0];
        } else if(cover_row_1[0] == cover_row_2[1]) {
          shared_row = cover_row_1[0];
          left_row = cover_row_1[1];
          right_row = cover_row_2[0];
        } else {
          continue;
        }
        if(left_row == right_row) {
          continue;
        }

        int left_col = search_context.base_house_idxs[c1];
        int right_col = search_context.base_house_idxs[c2];

        uint8_t left_idx = ROW_TO_IDXS[left_row][left_col];
        uint8_t right_idx = ROW_TO_IDXS[right_row][right_col];

        const uint8_t* left_peers = CELL_PEERS_LOOKUP[left_idx];
        const uint8_t* right_peers = CELL_PEERS_LOOKUP[right_idx];

        uint8_t intersections[20];
        int intersection_count = get_uint8_intersection(left_peers, right_peers, 20, 20, intersections);

        for(int i = 0; i < intersection_count; ++i) {
          if(intersections[i] == left_idx || intersections[i] == right_idx) {
            continue;
          }
          uint16_t removed = puzzle->candidates[intersections[i]] & candidate_mask;

          if(removed) {
            Step step = {
              .eliminated_mask = removed,
              .placed_val = 0,
              .target_cell = intersections[i],
              .technique = SKYSCRAPER
            };
            apply_step(puzzle, step);
            progress_made = true;
          }
        }
        if(progress_made) {
          return PROGRESS_MADE;
        }
      }
    }
  }
  return NO_PROGRESS;
}
