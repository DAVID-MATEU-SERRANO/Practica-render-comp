

set(UTAOS_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/test_pixel_aos.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_aos_logic.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_aos_ppm_smoke.cpp"
)

add_unit_test_target(
  TARGET_NAME   utaos
  SOURCE_FILES  ${UTAOS_TEST_SOURCES}
  LIBRARY_FILTER common
  COVERAGE_DIR  coverage-aos
  LIBRARY_TO_LINK common
  INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_SOURCE_DIR}/aos/include
)

# Si tu macro no llama internamente a gtest_discover_tests(),
# descomenta esto:
# find_package(GTest REQUIRED)
# gtest_discover_tests(utaos)
