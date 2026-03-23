add_library(puzzle STATIC 
  puzzle.c
)

target_include_directories(puzzle PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(puzzle PUBLIC logger)