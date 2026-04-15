add_library(algorithm_x_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/matrix.c
  ${CMAKE_CURRENT_SOURCE_DIR}/node.c
  ${CMAKE_CURRENT_SOURCE_DIR}/alg_x_solve.c
)

target_include_directories(algorithm_x_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(algorithm_x_solver PUBLIC puzzle logger)
