#include "common/include/parse_scene.hpp"
#include "common/include/scene.hpp"
#include <fstream>
#include <iostream>

int main() {
  std::ifstream in("scene.txt");
  if (!in) {
    std::cerr << "No se pudo abrir scene.txt\n";
    return 1;
  }

  Scene s;
  try {
    parse ::parse_scene_stream(in, s);
    std::cout << "OK\n";
    std::cout << "Mattes: " << s.mattes.size() << "\n";
    std::cout << "Metales: " << s.metales.size() << "\n";
    std::cout << "Refractarios: " << s.refractarios.size() << "\n";
    std::cout << "Esferas:    " << s.spheres.size() << "\n";
    std::cout << "Cilindros:  " << s.cylinders.size() << "\n";
  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  }
}
