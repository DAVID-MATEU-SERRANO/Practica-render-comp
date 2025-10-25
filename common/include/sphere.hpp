#ifndef RENDER_SPHERE_HPP
#define RENDER_SPHERE_HPP

#include "../include/matte.hpp"
#include "../include/metal.hpp"
#include "../include/point.hpp"
#include "../include/refractive.hpp"
#include <utility>
#include <variant>

namespace render {

  using t_material = std::variant<Matte, Metal, Refractive>;

  class Sphere {
  public:
    Sphere(Point sphere_center, double radius, t_material material)
        : sphere_center{sphere_center}, radius{radius}, material(std::move(material)) {
      // Introducir validaciones si es necesario, Falta incluir atributo que sea material
    }

    // [[nodiscard]] sirve para si haces operaciones y no se usan se eliminen (de momento las
    // dejamos pero no se si van a hacer falta)

    [[nodiscard]] double get_radius() const;
    [[nodiscard]] Point get_center() const;
    [[nodiscard]] t_material get_material() const;

  private:
    Point sphere_center;
    double radius;
    t_material material;
  };

}  // namespace render

#endif
