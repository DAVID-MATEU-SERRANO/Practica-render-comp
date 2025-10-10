#ifndef RENDER_MATTE_HPP
#define RENDER_MATTE_HPP

#include <string>
#include <utility>

namespace render {

  class Matte {
  public:
    Matte(std::string name, double reflec1, double reflec2, double reflec3)
        : name{std::move(name)}, reflec1(reflec1), reflec2(reflec2), reflec3(reflec3) { }

  private:
    std::string name;
    double reflec1;
    double reflec2;
    double reflec3;
  };

}  // namespace render

#endif
