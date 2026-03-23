add_library(puzzle_generator STATIC puzzle_generator.c)

target_include_directories(puzzle_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(puzzle_generator PUBLIC puzzle utils algorithm_x_solver)