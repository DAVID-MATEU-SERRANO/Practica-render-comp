#ifndef RENDER_MATERIAL_HPP
#define RENDER_MATERIAL_HPP

#include <string>

namespace render {

  class material {
  public:
    material(std::string type);

  private:
    std::string type;
  };

}  // namespace render

#endif
