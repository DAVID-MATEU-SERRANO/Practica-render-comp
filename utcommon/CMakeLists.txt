# utcommon/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

# Este include es parte de CMake, no de GTest
include(GoogleTest)

# Fuentes que necesita el runner de tests (producción)
set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/parse_config.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/parse_scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/pov.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/ray.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/logic.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/vector.cpp"
)

# Todos los tests
set(TEST_SRC_FILES 
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
  "${CMAKE_CURRENT_SOURCE_DIR}/test_ray.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_parse_config.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/test_logic.cpp"
)

# Ejecutable único de tests
add_executable(utcommon ${COMMON_SRC_FILES} ${TEST_SRC_FILES})

# Incluye headers (por si acaso)
target_include_directories(utcommon PRIVATE
  "${CMAKE_SOURCE_DIR}/common/include"
)

# Detecta el target correcto de GTest (según cómo lo haya creado el superbuild)
if (TARGET GTest::gtest_main)
  set(_GTEST_MAIN GTest::gtest_main)
elseif (TARGET gtest_main)
  set(_GTEST_MAIN gtest_main)
else()
  message(FATAL_ERROR "No se encontró el target de GoogleTest (gtest_main). Revisa el superbuild.")
endif()

target_link_libraries(utcommon PRIVATE common ${_GTEST_MAIN})

# Descubre automáticamente los TEST(...)
gtest_discover_tests(utcommon
  DISCOVERY_TIMEOUT 10
)
