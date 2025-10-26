#include "../../common/include/config.hpp"
#include "../../common/include/pov.hpp"
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
  std::ifstream file(arguments[1]);
  if (!file) {
    std::cerr << " No se pudo abrir config.txt\n";
    return 1;
  }
  std::ifstream in(arguments[2]);
  if (!in) {
    std::cerr << "No se pudo abrir scene.txt\n";
    return 1;
  }
  Scene scene;
  load_scene_from_files(scene, in, file);

  // Configuración
  std::cout << "CONFIGURACION:\n";
  std::cout << "samples_per_pixel: " << scene.get_samples_per_pixel() << "\n";
  std::cout << "max_depth: " << scene.get_max_depth() << "\n";
  std::cout << "material_rng_seed: " << scene.get_material_rng_seed() << "\n";
  std::cout << "rays_rng_seed: " << scene.get_rays_rng_seed() << "\n";
  std::cout << "gamma: " << scene.get_gamma() << "\n";
  std::cout << "background_dark_color: (" << scene.get_background_dark_color().get_r() << ", "
            << scene.get_background_dark_color().get_g() << ", "
            << scene.get_background_dark_color().get_b() << ")\n";
  std::cout << "background_light_color: (" << scene.get_background_light_color().get_r() << ", "
            << scene.get_background_light_color().get_g() << ", "
            << scene.get_background_light_color().get_b() << ")\n";

  // POV
  Pov const & pov = scene.get_pov();
  std::cout << "POV:\n";
  std::cout << "  camera_position: (" << pov.get_camera_position().get_x() << ", "
            << pov.get_camera_position().get_y() << ", " << pov.get_camera_position().get_z()
            << ")\n";
  std::cout << "  camera_target: (" << pov.get_camera_target().get_x() << ", "
            << pov.get_camera_target().get_y() << ", " << pov.get_camera_target().get_z() << ")\n";
  std::cout << "  camera_north: (" << pov.get_camera_north().get_x() << ", "
            << pov.get_camera_north().get_y() << ", " << pov.get_camera_north().get_z() << ")\n";
  std::cout << "  field_of_view: " << pov.get_field_of_view() << "\n";
  std::cout << "  image_width: " << pov.get_image_width() << "\n";
  std::cout << "  image_height: " << pov.get_image_height() << "\n";

  int image_height = scene.get_pov().get_image_height();
  int image_width  = scene.get_pov().get_image_width();
  std::size_t total_pixels =
      static_cast<std::size_t>(image_height) * static_cast<std::size_t>(image_width);
  std::vector<Pixel> pixels(total_pixels);
  std::ofstream ppm_file(arguments[3]);
  ppm_file << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int f = 0; f < image_height; ++f) {
    for (int c = 0; c < image_width; ++c) {
      Pixel pixel       = scene.get_pixel_color(f, c);
      std::size_t index = static_cast<std::size_t>(f) * static_cast<std::size_t>(image_height) +
                          static_cast<std::size_t>(c);
      pixels[index] = pixel;
      ppm_file << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
               << static_cast<int>(pixel.b) << "\n";
      std::cout << f << " " << c << " PIXEL COUNTER  \n";
      std::cout.flush();
    }
  }
  ppm_file.close();

  return 0;
}
