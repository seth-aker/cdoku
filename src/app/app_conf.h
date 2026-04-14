#ifndef SRC_APP_MAIN_H
#define SRC_APP_MAIN_H
#include "puzzle.h"
#include "log.h"
#define MAX_PUZZLE_COUNT 1000

static const char* const difficulty_strs[] = {
  "BEGINNER",
  "EASY",
  "MEDIUM",
  "HARD",
  "IMPOSSIBLE"
};

typedef struct AppConfig {
  int puzzle_count;
  DiffRating target_difficulty;
} AppConfig;

void parse_args(int argc, const char* argv[], AppConfig* conf);
int get_puzzle_count(const char* str);
bool is_valid_puzzle_str(const char* str);
bool is_valid_difficulty_target(const char* str);
void setup_logger(LogLevel level);
#endif
