#include "../../common/include/config.hpp"
#include "../../common/include/scene.hpp"
#include <fstream>
#include <iostream>
#include <vector>

int main() {
  // #### PARSE ####//

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

  std::vector<int> R(total_pixels);
  std::vector<int> G(total_pixels);
  std::vector<int> B(total_pixels);

  for (int f = 0; f < scene.get_pov().get_image_height(); ++f) {
    for (int c = 0; c < scene.get_pov().get_image_width(); ++c) {
      Pixel pixel                    = scene.get_pixel_color(f, c);
      R[static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
        static_cast<std::size_t>(c)] = pixel.r;
      G[static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
        static_cast<std::size_t>(c)] = pixel.g;
      B[static_cast<std::size_t>(f) * static_cast<std::size_t>(image_width) +
        static_cast<std::size_t>(c)] = pixel.b;
    }
  }
  return 0;
}
