add_library(algorithm_x_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/matrix.c
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/node.c
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/alg_x_solve.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../puzzle/puzzle.c
)

target_include_directories(algorithm_x_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/../puzzle
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x
  ${CMAKE_CURRENT_SOURCE_DIR}/../logger
)