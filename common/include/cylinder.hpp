#ifndef RENDER_CYLINDER_HPP
#define RENDER_CYLINDER_HPP

#include "../include/matte.hpp"
#include "../include/metal.hpp"
#include "../include/point.hpp"
#include "../include/refractive.hpp"
#include "../include/vector.hpp"
#include <utility>
#include <variant>

namespace render {

  using t_material = std::variant<Matte, Metal, Refractive>;

  class Cylinder {
  public:
    Cylinder(Point vec_center, double radius, Vector vec, t_material material)
        : vec_center{vec_center}, radius{radius}, vec_edge{vec}, material(std::move(material)) { }

    // [[nodiscard]] sirve para si haces operaciones y no se usan se eliminen (de momento las
    // dejamos pero no se si van a hacer falta)
    [[nodiscard]] Point get_center() const;
    [[nodiscard]] Vector get_edge() const;
    [[nodiscard]] double get_radius() const;
    [[nodiscard]] double get_height() const;
    [[nodiscard]] t_material get_material() const;

  private:
    Point vec_center;
    double radius;
    Vector vec_edge;
    t_material material;
  };

}  // namespace render

#endif
