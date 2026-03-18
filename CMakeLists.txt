cmake_minimum_required(VERSION 3.28)

project(CDOKU C)

add_subdirectory(external)

add_subdirectory(tests)

add_subdirectory(src/solvers)

# add_executable(algorithm_x_solver src/solvers/algorithm_x/alg_x_solve.c)




# add_executable(test_suite tests/test_suite.c)

# target_link_libraries(test_suite PRIVATE unity sudoku_solver)

# add_executable(puzzle_generator src/generator/generate_puzzle.c)

# target_include_directories(puzzle_generator PRIVATE src/generator)

# target_link_libraries(puzzle_generator PRIVATE sudoku_solver m)
