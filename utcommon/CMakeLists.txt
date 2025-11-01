set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/parse_config.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/parse_scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/pov.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/ray.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/render.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/sphere.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/util.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/vector.cpp"
)

set(CURRENT_DIR_SRC_FILES 
  # AÑADIR TESTS AQUI
  "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
)

add_unit_test_target(
  TARGET_NAME utcommon
  SOURCE_FILES ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER common
  COVERAGE_DIR coverage-common
  LIBRARY_TO_LINK common
)
