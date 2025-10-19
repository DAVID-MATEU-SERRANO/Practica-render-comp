#include "point.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  class Proyection_window {
  public:
    Proyection_window(Vector focal_vector, double focal_distance, double height, double width,
                      Vector horizontal_vector, Vector vertical_vector, Point origin)
        : focal_vector{focal_vector}, focal_distance{focal_distance}, height{height}, width{width},
          horizontal_vector{horizontal_vector}, vertical_vector{vertical_vector}, origin{origin} { }

    // Getters para los atributos
    [[nodiscard]] Vector get_focal_vector() const;
    [[nodiscard]] double get_focal_distance() const;
    [[nodiscard]] double get_height() const;
    [[nodiscard]] double get_width() const;
    [[nodiscard]] Vector get_horizontal_vector() const;
    [[nodiscard]] Vector get_vertical_vector() const;
    [[nodiscard]] Point get_origin() const;

  private:
    Vector focal_vector;
    double focal_distance;
    double height;
    double width;
    Vector horizontal_vector;
    Vector vertical_vector;
    Point origin;
  };

}  // namespace render
