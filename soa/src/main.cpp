#include "../../common/include/config.hpp"
#include "../../common/include/render.hpp"
#include "../../common/include/scene.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace render;

int main(int argc, char * argv[]) {
  std::vector<std::string> arguments(argv, argv + argc);
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments: " << (argc - 1) << "\n";
    std::cerr << "Usage: " << arguments[0] << " <config_file> <output_file>\n";
    return 1;
  }
  std::ifstream in(arguments[1]);
  std::ifstream file(arguments[2]);
  if (!in or !file) {
    std::cerr << "Error al abrir archivos\n";
    return 1;
  }
  Scene scene;
  load_scene_from_files(scene, in, file);
  int image_height = scene.get_pov().get_image_height();
  int image_width  = scene.get_pov().get_image_width();
  std::size_t total_pixels =
      static_cast<std::size_t>(image_height) * static_cast<std::size_t>(image_width);
  std::vector<uint8_t> R(total_pixels);
  std::vector<uint8_t> G(total_pixels);
  std::vector<uint8_t> B(total_pixels);
  // Creación del archivo PPM
  std::ofstream ppm_file(arguments[3]);
  ppm_file << "P3\n" << image_width << " " << image_height << "\n255\n";

  std::mt19937_64 rng(scene.get_rays_rng_seed());
  std::mt19937_64 m_rng(scene.get_material_rng_seed());

  for (int f = 0; f < image_height; ++f) {
    for (int c = 0; c < image_width; ++c) {
      Pixel pixel       = scene.get_pixel_color(f, c, rng, m_rng);
      std::size_t index = static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
                          static_cast<std::size_t>(c);
      R[index] = pixel.r;
      G[index] = pixel.g;
      B[index] = pixel.b;
      ppm_file << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
               << static_cast<int>(pixel.b) << "\n";
    }
  }
  ppm_file.close();

  return 0;
}
