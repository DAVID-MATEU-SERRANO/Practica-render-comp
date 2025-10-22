// Estructura de datos para almacenar la informacion de una escena
// Contiene los parametros que se obtienen de leer el archivo scene.txt

#include "color.hpp"
#include "cylinder.hpp"
#include "pov.hpp"
#include "sphere.hpp"
#include <sys/types.h>
#include <utility>
#include <vector>

namespace render {

  struct Pixel {
    int r;
    int g;
    int b;
  };

  class Scene {
    /*
    std::vector<render::Matte> mattes;             // Vector Matte
    std::vector<render::Metal> metales;            // Vector Metal
    std::vector<render::Refractive> refractarios;  // Vector Refractive*/

    // Clase usada para el punto de vista
  public:
    Scene(std::vector<render::Sphere> spheres, std::vector<render::Cylinder> cylinders,
          render::Pov pov, int samples_per_pixel, int max_depth, uint64_t material_rng_seed,
          uint64_t rays_rng_seed, render::Color background_dark_color,
          render::Color background_light_color)
        : spheres{std::move(spheres)}, cylinders{std::move(cylinders)}, pov{pov},
          samples_per_pixel{samples_per_pixel}, max_depth{max_depth},
          material_rng_seed{material_rng_seed}, rays_rng_seed{rays_rng_seed},
          background_dark_color{(background_dark_color)},
          background_light_color{background_light_color} { }

    // Método
    [[nodiscard]] Pixel get_pixel_color(int f, int c) const;

  private:
    std::vector<render::Sphere> spheres;      // Vector de esferas
    std::vector<render::Cylinder> cylinders;  // Vector de cilindros
    render::Pov pov;                          // Cámara

    int samples_per_pixel;       // Muestras por pixel
    int max_depth;               // Profundidad maxima de rayos
    uint64_t material_rng_seed;  // Semilla para el generador de numeros aleatorios de materiales
    uint64_t rays_rng_seed;      // Semilla para el generador de numeros aleatorios de rayos
    render::Color background_dark_color;   // Color oscuro del fondo
    render::Color background_light_color;  // Color claro del fondo

    /*
    // Mapa para buscar materiales por su nombre
    std::unordered_map<std::string, size_t> material_index;

    // FUNCIONES PARA AGREGAR MATERIALES Y OBJETOS A LA ESCENA
    // Agregar material a la escena (verificar que no exista otro material con el mismo nombre)

    void add_material_Matte(render::Matte const & m, std::string const & lineforprint) {
      if (material_index.find(m.get_name()) != material_index.end()) {
        std::ostringstream oss;
        oss << "Material with name '[" << m.get_name() << "]' already exists\n"
            << "Line: " << lineforprint;
        throw std::runtime_error(oss.str());
      }
      material_index[m.get_name()] = mattes.size() * 10 + 0;
      mattes.push_back(m);
    }

    void add_material_Metal(render::Metal const & m, std::string const & lineforprint) {
      if (material_index.find(m.get_name()) != material_index.end()) {
        std::ostringstream oss;
        oss << "Material with name '[" << m.get_name() << "]' already exists\n"
            << "Line: " << lineforprint;
        throw std::runtime_error(oss.str());
      }
      material_index[m.get_name()] = metales.size() * 10 + 1;
      metales.push_back(m);
    }

    void add_material_Refractive(render::Refractive const & m, std::string const & lineforprint) {
      if (material_index.find(m.get_name()) != material_index.end()) {
        std::ostringstream oss;
        oss << "Material with name '[" << m.get_name() << "]' already exists\n"
            << "Line: " << lineforprint;
        throw std::runtime_error(oss.str());
      }
      material_index[m.get_name()] = refractarios.size() * 10 + 2;
      refractarios.push_back(m);
    }

    void add_sphere(render::Sphere const & sphere) { spheres.push_back(sphere); }

    void add_cylinder(render::Cylinder const & cylinder) { cylinders.push_back(cylinder); }

    void clear() {
      metales.clear();
      mattes.clear();
      refractarios.clear();
      spheres.clear();
      cylinders.clear();
      material_index.clear();
    }*/
  };

}  // namespace render
