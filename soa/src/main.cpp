#include "../../common/include/config.hpp"
#include "../../common/include/scene.hpp"
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Error: Invalid number of arguments: " << (argc - 1) << "\n";
    std::cerr << "Usage: " << argv[0] << " <config_file> <output_file>\n";
    return 1;
  }

  // ### IMAGE_GENERATION ### //
  using namespace render;
  // TODO: todos estos datos son temporales de prueba para q no me den errores. Cuando tengáis el
  // parser listo se quita todo y simplemente se pone Scene scene = LO q devuelva el parser

  //  ----- Datos vacíos o de prueba -----
  std::vector<Sphere> spheres;
  std::vector<Cylinder> cylinders;

  // Tamaño de la imagen
  ImageSize img_size{800, 600};

  // Parámetros de cámara (punto de vista)
  Point camera_position(0.0, 0.0, 0.0);
  Point camera_target(0.0, 0.0, -1.0);
  Vector camera_north(0.0, 1.0, 0.0);
  double field_of_view = 60.0;  // grados

  Pov pov(camera_position, camera_target, camera_north, field_of_view, img_size);

  // Parámetros de render
  int samples_per_pixel      = 10;
  int max_depth              = 5;
  uint64_t material_rng_seed = 12'345;
  uint64_t rays_rng_seed     = 67'890;
  Color background_dark_color(0, 0, 0);
  Color background_light_color(255, 255, 255);

  // Crear escena temporal para que compile
  Scene scene(spheres, cylinders, pov, samples_per_pixel, max_depth, material_rng_seed,
              rays_rng_seed, background_dark_color, background_light_color);

  int image_height = scene.get_pov().get_image_height();
  int image_width  = scene.get_pov().get_image_width();
  std::size_t total_pixels =
      static_cast<std::size_t>(image_height) * static_cast<std::size_t>(image_width);

  std::vector<uint8_t> R(total_pixels);
  std::vector<uint8_t> G(total_pixels);
  std::vector<uint8_t> B(total_pixels);

  // Creación del archivo PPM
  std::ofstream ppm_file(argv[2]);
  ppm_file << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int f = 0; f < image_height; ++f) {
    for (int c = 0; c < image_width; ++c) {
      Pixel pixel       = scene.get_pixel_color(f, c);
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
