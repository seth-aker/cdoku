#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "app_conf.h"
#include "puzzle_generator.h"
#include "puzzle.h"
#include "log.h"
int main(int argc, const char* argv[]) {
  AppConfig config;
  parse_args(argc, argv, &config);
  Puzzle puzzle;
  char puzzle_str_buff[100];
  for(int i = 0; i < config.puzzle_count; ++i) {
    memset(&puzzle, 0, sizeof(Puzzle));
    generate_puzzle(&puzzle, config.target_difficulty);
    int written = stringify_puzzle(puzzle_str_buff, 100, &puzzle);
    if(written < 0 || (size_t)written >= sizeof(puzzle_str_buff)) {
      log_error("An error occured writting generated puzzle to string!");
      exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%s", puzzle_str_buff);
  }
  return EXIT_SUCCESS;
}
