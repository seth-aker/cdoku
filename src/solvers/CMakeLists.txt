add_library(algorithm_x_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/matrix.c
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/node.c
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/alg_x_solve.c
)

target_include_directories(algorithm_x_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x
)

target_link_libraries(algorithm_x_solver PUBLIC puzzle logger)



add_library(logical_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/logical/singles.c
  ${CMAKE_CURRENT_SOURCE_DIR}/logical/logical_solve.c
  ${CMAKE_CURRENT_SOURCE_DIR}/logical/subsets.c
  ${CMAKE_CURRENT_SOURCE_DIR}/logical/locked_candidates.c
)
target_include_directories(logical_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/logical
)

target_link_libraries(logical_solver PUBLIC puzzle logger utils)
