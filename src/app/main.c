#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "app_conf.h"
#include "puzzle_generator.h"
#include "puzzle.h"
#include "log.h"
int main(int argc, const char* argv[]) {
  if(argc != 3) {
    log_error("Invalid number of arguments. Expected 2, received: %d", argc - 1);
    return EXIT_FAILURE;
  }

#ifndef NDEBUG
  srand(123456);
  log_set_level(LOG_TRACE);
  setup_logger(LOG_TRACE);
#else
  srand((unsigned)time(NULL));
  log_set_level(LOG_ERROR);
  setup_logger(LOG_ERROR);
#endif
  AppConfig config;
  parse_args(argc, argv, &config);
  Puzzle puzzle;
  char puzzle_str_buff[110];
  for(int i = 0; i < config.puzzle_count; ++i) {
    memset(&puzzle, 0, sizeof(Puzzle));
    generate_puzzle(&puzzle, config.target_difficulty);
    int written = stringify_puzzle(puzzle_str_buff, 110, &puzzle);
    if(written < 0 || (size_t)written >= sizeof(puzzle_str_buff)) {
      log_error("An error occured writting generated puzzle to string!");
      exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%s\n", puzzle_str_buff);
  }
  return EXIT_SUCCESS;
}
