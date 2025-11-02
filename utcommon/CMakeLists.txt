set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/parse_config.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/parse_scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/pov.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/ray.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/render.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/vector.cpp"
)

set(CURRENT_DIR_SRC_FILES 
  # AÑADIR TESTS AQUI
  "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_color.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_cylinder.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_matte.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_metal.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_parse_exception.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_point.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_pov.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_proyection_window.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_parse_scene.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_refractive.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_sphere.cpp"
)

add_unit_test_target(
  TARGET_NAME utcommon
  SOURCE_FILES ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER common
  COVERAGE_DIR coverage-common
  LIBRARY_TO_LINK common
)

add_executable(utest-common ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES})

target_link_libraries(utest-common PRIVATE common GTest::gtest_main)

gtest_discover_tests(utest-common)