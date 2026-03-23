add_library(algorithm_x_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/matrix.c
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/node.c
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x/alg_x_solve.c
)

target_include_directories(algorithm_x_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_x
)

target_link_libraries(algorithm_x_solver PUBLIC puzzle logger)