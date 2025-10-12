#ifndef RENDER_SPHERE_HPP
#define RENDER_SPHERE_HPP

#include "matte.hpp"
#include "metal.hpp"
#include "refractive.hpp"
#include <utility>
#include <variant>

namespace render {

  using t_material = std::variant<Matte, Metal, Refractive>;

  class Sphere {
  public:
    Sphere(double cords_x, double cords_y, double cords_z, double radius, t_material material)
        : cords_x{cords_x}, cords_y{cords_y}, cords_z{cords_z}, radius{radius},
          material(std::move(material)) {
      // Introducir validaciones si es necesario, Falta incluir atributo que sea material
    }

    // [[nodiscard]] sirve para si haces operaciones y no se usan se eliminen (de momento las
    // dejamos pero no se si van a hacer falta)
    [[nodiscard]] double get_cords_x() const;
    [[nodiscard]] double get_cords_y() const;
    [[nodiscard]] double get_cords_z() const;
    [[nodiscard]] double get_radius() const;

  private:
    double cords_x;
    double cords_y;
    double cords_z;
    double radius;
    t_material material;
  };

}  // namespace render

#endif
