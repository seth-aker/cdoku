add_executable(test_matrix ${CMAKE_CURRENT_SOURCE_DIR}/solvers/algorithm_x/test_matrix.c)

target_link_libraries(test_matrix unity algorithm_x_solver)

add_test(test_matrix_tests test_matrix)