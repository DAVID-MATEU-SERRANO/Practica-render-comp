#include "common/include/config.hpp"
#include "common/include/parse_config.hpp"
#include <fstream>
#include <iostream>

int main() {
  Config cfg;

  std::ifstream file("config.txt");
  if (!file) {
    std::cerr << "❌ No se pudo abrir config.txt\n";
    return 1;
  }

  try {
    parse2::parse_config_stream(file, cfg);
  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  std::cout << "Config leída correctamente\n";
  std::cout << "field_of_view: " << cfg.field_of_view << "\n";
  std::cout << "image_height: " << cfg.image_width << "x" << cfg.image_height() << "\n";
  std::cout << "gamma: " << cfg.gamma << "\n";
  std::cout << "material_rng_seed: " << cfg.material_rng_seed << "\n";
}
