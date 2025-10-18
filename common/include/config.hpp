// Estructura para almacenar la configuracion de la escena
// Cntiene los parametros que se obtienen de leer config.txt

//============================================================//

/* Formato de config.txt:
# --- Tamaño de imagen ---
aspect_ratio_width  = 16
aspect_ratio_height = 9
image_width         = 800

# --- Cámara ---
camera_position = 0 0 -10
camera_target   = 0 0 0
camera_up       = 0 1 0
fov_deg         = 60

# --- Renderizado ---
samples_per_pixel = 50
max_depth         = 10
gamma             = 2.2

# --- Color de fondo ---
background_dark  = 0.25 0.5 1.0
background_light = 1.0 1.0 1.0

# --- Semillas ---
material_seed = 13
ray_seed      = 19
*/

#include <array>
#include <cmath>
#include <string>

struct Config {
  // Tamaño de imagen
  int aspect_ratio_width  = 16;
  int aspect_ratio_height = 9;
  int image_width         = 800;

  // Cámara
  std::array<double, 3> camera_position{0.0, 0.0, -10.0};
  std::array<double, 3> camera_target{0.0, 0.0, 0.0};
  std::array<double, 3> camera_up{0.0, 1.0, 0.0};
  double fov_deg = 60.0;

  // Renderizado
  int samples_per_pixel = 50;
  int max_depth         = 10;
  double gamma          = 2.2;

  // Color de fondo
  std::array<double, 3> background_dark{0.25, 0.5, 1.0};
  std::array<double, 3> background_light{1.0, 1.0, 1.0};

  // Semillas
  unsigned int material_seed = 13;
  unsigned int ray_seed      = 19;

  [[nodiscard]] int image_height() const {
    return static_cast<int>(
        std::round(image_width * double(aspect_ratio_height) / double(aspect_ratio_width)));
  }
};
