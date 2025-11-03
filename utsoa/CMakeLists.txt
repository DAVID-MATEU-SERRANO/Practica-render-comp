cmake_minimum_required(VERSION 3.20)


set(UTSOA_TESTS
  ${CMAKE_CURRENT_SOURCE_DIR}/test_soa.cpp
)

add_executable(utsoa ${UTSOA_TESTS})

target_link_libraries(utsoa
  PRIVATE
    common
    GTest::gtest_main
)

target_include_directories(utsoa
  PRIVATE
    ${CMAKE_SOURCE_DIR}/soa/include
    ${CMAKE_SOURCE_DIR}/common/include
)

include(GoogleTest)
gtest_discover_tests(utsoa)

set_target_properties(utsoa PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/utsoa/$<CONFIG>
)
