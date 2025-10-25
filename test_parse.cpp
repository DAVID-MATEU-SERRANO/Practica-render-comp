#include "common/include/render.hpp"
#include "common/include/scene.hpp"
#include <fstream>
#include <iostream>

int main() {
  std::ifstream in("scene.txt");
  if (!in) {
    std::cerr << "No se pudo abrir scene.txt\n";
    return 1;
  }
  std::ifstream file("config.txt");
  if (!file) {
    std::cerr << " No se pudo abrir config.txt\n";
    return 1;
  }

  render::Scene scene;
  render::load_scene_from_files(scene, in, file);

  std::cout << "gamma: " << scene.get_gamma() << "\n";
  std::cout << "height: " << scene.get_pov().get_image_height() << "\n";
  std::cout << "vector esferas (count): " << scene.get_vector_esferas().size() << "\n";

  return 0;
}
