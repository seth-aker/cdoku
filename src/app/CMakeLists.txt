add_executable(puzzle_generator_app main.c app_conf.c)

target_include_directories(puzzle_generator_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(puzzle_generator_app -static puzzle_generator)
