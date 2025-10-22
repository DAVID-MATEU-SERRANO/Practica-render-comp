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
    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] double get_reflect1() const;

    [[nodiscard]] double get_reflect2() const;

    [[nodiscard]] double get_reflect3() const;

  private:
    std::string name;
    double reflec1;
    double reflec2;
    double reflec3;
  };

}  // namespace render

#endif
