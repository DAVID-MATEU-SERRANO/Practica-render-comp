#ifndef RENDER_CYLINDER_HPP
#define RENDER_CYLINDER_HPP

#include "matte.hpp"
#include "metal.hpp"
#include "refractive.hpp"
#include "vector.hpp"
#include <utility>
#include <variant>

namespace render {

  using t_material = std::variant<Matte, Metal, Refractive>;

  class Cylinder {
  public:
    Cylinder(Vector vec_center, double radius, Vector vec, t_material material)
        : vec_center{vec_center}, radius{radius}, vec_edge{vec}, material(std::move(material)) {
      // Introducir validaciones si es necesario
    }

    // [[nodiscard]] sirve para si haces operaciones y no se usan se eliminen (de momento las
    // dejamos pero no se si van a hacer falta)
    [[nodiscard]] double get_cords_x() const;
    [[nodiscard]] double get_cords_y() const;
    [[nodiscard]] double get_cords_z() const;
    [[nodiscard]] double get_radius() const;
    [[nodiscard]] double get_height() const;

  private:
    Vector vec_center;
    double radius;
    Vector vec_edge;
    t_material material;
  };

}  // namespace render

#endif
