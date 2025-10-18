#include <gtest/gtest.h>

#include "vector.hpp"

TEST(test_vector, magnitude_zero) {
  render::Vector vec{0.0, 0.0, 0.0};
  EXPECT_EQ(vec.magnitude(), 0.0);
}

TEST(test_vector, magnitude_positive) {
  render::Vector vec{3.0, 4.0, 0.0};
  EXPECT_EQ(vec.magnitude(), 5.0);
}
