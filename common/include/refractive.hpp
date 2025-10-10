#ifndef RENDER_REFRACTIVE_HPP
#define RENDER_REFRACTIVE_HPP

#include <string>
#include <utility>

namespace render {

  class Refractive {
  public:
    Refractive(std::string name, double refraction_index)
        : name{std::move(name)}, refraction_index(refraction_index) { }

    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] double get_refraction_index() const;

  private:
    std::string name;
    double refraction_index;
  };

}  // namespace render

#endif
