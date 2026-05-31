add_library(logical_solver STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/singles.c
  ${CMAKE_CURRENT_SOURCE_DIR}/logical_solve.c
  ${CMAKE_CURRENT_SOURCE_DIR}/subsets.c
  ${CMAKE_CURRENT_SOURCE_DIR}/locked_candidates.c
  ${CMAKE_CURRENT_SOURCE_DIR}/x_wing.c
  ${CMAKE_CURRENT_SOURCE_DIR}/skyscraper.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xy_wing.c
  ${CMAKE_CURRENT_SOURCE_DIR}/simple_colors.c
  ${CMAKE_CURRENT_SOURCE_DIR}/chute_remote_pairs.c
  ${CMAKE_CURRENT_SOURCE_DIR}/guess.c
  ${CMAKE_CURRENT_SOURCE_DIR}/chain_utils.c
  ${CMAKE_CURRENT_SOURCE_DIR}/w_wing.c
  ${CMAKE_CURRENT_SOURCE_DIR}/swordfish.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xyz_wing.c
)
target_include_directories(logical_solver PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(logical_solver PUBLIC puzzle logger utils)
