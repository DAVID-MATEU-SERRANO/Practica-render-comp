#include "object.hpp"
#include <gtest/gtest.h>

namespace {

  TEST(test_object, create_sphere) {
    render::object obj("sphere", 1.0, 2.0, 3.0, 4.0);
    // Aquí podrías agregar más verificaciones si la clase object tuviera métodos para obtener sus
    // propiedades
  }

  TEST(test_object, create_cilindro) {
    render::object obj("cilindro", 1.0, 2.0, 3.0, 4.0);
    // Aquí podrías agregar más verificaciones si la clase object tuviera métodos para obtener sus
    // propiedades
  }

  TEST(test_object, invalid_radius) {
    EXPECT_THROW(render::object obj("sphere", 1.0, 2.0, 3.0, -4.0), std::invalid_argument);
  }

  TEST(test_object, invalid_type) {
    EXPECT_THROW(render::object obj("cube", 1.0, 2.0, 3.0, 4.0), std::invalid_argument);
  }

  TEST(test_object, negative_coordinates) {
    EXPECT_THROW(render::object obj("sphere", -1.0, 2.0, 3.0, 4.0), std::invalid_argument);
  }

}  // namespace
