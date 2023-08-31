project(mesh_tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/mesh/bin)

add_executable(mesh test_mesh.cpp)
target_include_directories(mesh PUBLIC ${CMAKE_SOURCE_DIR}/fem)
target_link_libraries(mesh fem)

add_executable(element test_element.cpp)
target_include_directories(element PUBLIC ${CMAKE_SOURCE_DIR}/fem)
target_link_libraries(element fem)

