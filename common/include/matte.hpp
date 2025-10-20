#ifndef RENDER_MATTE_HPP
#define RENDER_MATTE_HPP

#include <string>
#include <utility>

namespace render {

  class Matte {
  public:
    Matte(std::string name, double reflec1, double reflec2, double reflec3)
        : name{std::move(name)}, reflec1(reflec1), reflec2(reflec2), reflec3(reflec3) { }

    // Getters
    [[nodiscard]] std::string get_name() const { return name; }

    [[nodiscard]] double get_reflect1() const { return reflec1; }

    [[nodiscard]] double get_reflect2() const { return reflec2; }

    [[nodiscard]] double get_reflect3() const { return reflec3; }

    // Setters
    void set_name(std::string const & new_name) { name = new_name; }

    void set_reflect1(double new_reflec1) { reflec1 = new_reflec1; }

    void set_reflect2(double new_reflec2) { reflec2 = new_reflec2; }

    void set_reflect3(double new_reflec3) { reflec3 = new_reflec3; }

  private:
    std::string name;
    double reflec1;
    double reflec2;
    double reflec3;
  };

}  // namespace render

#endif
