add_library(logical_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/singles.c
  ${CMAKE_CURRENT_SOURCE_DIR}/logical_solve.c
  ${CMAKE_CURRENT_SOURCE_DIR}/subsets.c
  ${CMAKE_CURRENT_SOURCE_DIR}/locked_candidates.c
  ${CMAKE_CURRENT_SOURCE_DIR}/x_wing.c
  ${CMAKE_CURRENT_SOURCE_DIR}/skyscraper.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xy_wing.c
  ${CMAKE_CURRENT_SOURCE_DIR}/guess.c
)
target_include_directories(logical_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(logical_solver PUBLIC puzzle logger utils)
