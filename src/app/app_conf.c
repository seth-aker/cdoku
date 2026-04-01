#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "app_conf.h"
#include "log.h"

void parse_args(int argc, const char* argv[], AppConfig* conf) {
  if(!is_valid_difficulty_target(argv[1])) {
    log_error("Invalid difficulty string");
    exit(EXIT_FAILURE);
  }
  int puzzle_count = get_puzzle_count(argv[2]);
  if(puzzle_count < 1) {
    exit(EXIT_FAILURE);
  }

  conf->puzzle_count = puzzle_count;
  switch(argv[1][0]) {
  case 'B':
    conf->target_difficulty = BEGINNER;
    break;
  case 'E':
    conf->target_difficulty = EASY;
    break;
  case 'M':
    conf->target_difficulty = MEDIUM;
    break;
  case 'H':
    conf->target_difficulty = HARD;
    break;
  case 'I':
    conf->target_difficulty = IMPOSSIBLE;
    break;
  default:
    exit(EXIT_FAILURE);
    break;
  }
}

int get_puzzle_count(const char* str) {
  char* endptr;
  errno = 0;
  int val = (int)strtol(str, &endptr, 10);
  if(errno == ERANGE || *endptr != '\0' || val > MAX_PUZZLE_COUNT) {
    log_error("Invalid puzzle count request. Must be a number between 0 and 1000");
    return -1;
  } else {
    return val;
  }
}
bool is_valid_puzzle_str(const char* str) {
  int len = 0;
  for(int i = 0; str[i] != '\0'; ++i) {
    if(!isdigit((unsigned char)str[i])) {
      return false;
    } else {
      len++;
    }
  }
  return len == 81;
}
bool is_valid_difficulty_target(const char* str) {
  for(int i = 0; i < 5; ++i) {
    if(strcmp(str, difficulty_strs[i]) == 0) {
      return true;
    }
  }
  return false;
}
