#include "scene.hpp"
#include <fstream>

namespace render {

  bool load_scene_from_files(render::Scene & scene, std::ifstream & in, std::ifstream & file);

}  // namespace render

/*
int main() {
  render::Scene escena;                              // aquí se crea el Scene
  if (!render::load_scene_from_files(escena)) {
    std::cerr << "Error cargando escena\n";
    return 1;
  }

  // usar `escena`...
  return 0;
}
*/
