#include "common/include/parse_config.hpp"
#include "common/include/scene.hpp"
#include <fstream>
#include <iostream>

int main() {
  render::Scene scene;

  std::ifstream file("config.txt");
  if (!file) {
    std::cerr << "No se pudo abrir config.txt\n";
    return 1;
  }

  try {
    parse::parse_config_stream(file, scene);
  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  auto pov = scene.get_pov();
  std::cout << "Config leída correctamente\n";
  std::cout << "field_of_view: " << pov.get_field_of_view() << "\n";
  std::cout << "image_size: " << pov.get_image_width() << "x" << pov.get_image_height() << "\n";
  std::cout << "material_rng_seed: " << scene.get_material_rng_seed() << "\n";
  std::cout << "rays_rng_seed: " << scene.get_rays_rng_seed() << "\n";
  std::cout << "gamma: " << scene.get_gamma() << "\n";
}
