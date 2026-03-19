include(CTest)
enable_testing()

add_executable(test_matrix ${CMAKE_CURRENT_SOURCE_DIR}/solvers/algorithm_x/test_matrix.c)
add_executable(test_node ${CMAKE_CURRENT_SOURCE_DIR}/solvers/algorithm_x/test_node.c)

target_link_libraries(test_matrix unity algorithm_x_solver)
target_link_libraries(test_node unity algorithm_x_solver)

add_test(matrix_tests test_matrix)
add_test(node_tests test_node)
