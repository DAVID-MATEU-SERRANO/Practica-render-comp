#include <print>

#include "vector.hpp"

int main() {
  std::println("Starting AOS rendering");
  render::Vector vec{1.0, 2.0, 3.0};
  std::println("Vector magnitude: {}", vec.magnitude());
}
