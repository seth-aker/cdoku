cmake_minimum_required(VERSION 3.28)

project(CDOKU C)

add_library(sudoku_solver STATIC
  external/log.c/log.c
  src/logger/logger.c
  src/puzzle/puzzle.c
  src/puzzle/step.c
  src/puzzle/strategies.c
  src/solver/basic_fish_solver.c
  src/solver/finned_fish_solver.c
  src/solver/skyscraper_solver.c
  src/solver/sudoku_solver.c
)

target_include_directories(sudoku_solver PUBLIC 
  external/log.c
  src
  src/logger
  src/puzzle
  src/solvers
)

add_library(unity STATIC external/unity/unity.c)

target_include_directories(unity PUBLIC external/unity)

include(CTest)
enable_testing()

add_executable(test_suite tests/test_suite.c)

target_link_libraries(test_suite PRIVATE unity sudoku_solver)

add_executable(puzzle_generator src/generator/generate_puzzle.c)

target_include_directories(puzzle_generator PRIVATE src/generator)

target_link_libraries(puzzle_generator PRIVATE sudoku_solver m)
