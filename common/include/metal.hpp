#ifndef RENDER_METAL_HPP
#define RENDER_METAL_HPP

#include <string>
#include <utility>

namespace render {

  class Metal {
  public:
    Metal(std::string name, double reflec1, double reflec2, double reflec3, double difusion_factor)
        : name{std::move(name)}, reflec1(reflec1), reflec2(reflec2), reflec3(reflec3),
          difusion_factor(difusion_factor) { }

    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] double get_reflect1() const;
    [[nodiscard]] double get_reflect2() const;
    [[nodiscard]] double get_reflect3() const;
    [[nodiscard]] double get_difusion_factor() const;

  private:
    std::string name;
    double reflec1;
    double reflec2;
    double reflec3;
    double difusion_factor;
  };

}  // namespace render

#endif
