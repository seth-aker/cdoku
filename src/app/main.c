#include "app_conf.h"
#include "puzzle_generator.h"
int main(int argc, const char* argv[]) {
  AppConfig config;
  parse_args(argc, argv, &config);

  for(int i = 0; i < config.puzzle_count; ++i) {


  }

}
