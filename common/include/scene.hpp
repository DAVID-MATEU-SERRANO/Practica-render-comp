// Estructura de datos para almacenar la informacion de una escena
// Contiene los parametros que se obtienen de leer el archivo scene.txt

#include "color.hpp"
#include "cylinder.hpp"
#include "matte.hpp"
#include "metal.hpp"
#include "parse_exception.hpp"
#include "pov.hpp"
#include "ray.hpp"
#include "refractive.hpp"
#include "sphere.hpp"
#include <map>
#include <sys/types.h>
#include <utility>
#include <vector>

namespace render {

  struct Pixel {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
  };

  class Scene {
    /*
    std::vector<render::Matte> mattes;             // Vector Matte
    std::vector<render::Metal> metales;            // Vector Metal
    std::vector<render::Refractive> refractarios;  // Vector Refractive*/

    // Clase usada para el punto de vista
  public:
    Scene(std::vector<Sphere> spheres, std::vector<Cylinder> cylinders, Pov pov,
          int samples_per_pixel, int max_depth, uint64_t material_rng_seed, uint64_t rays_rng_seed,
          Color background_dark_color, Color background_light_color)
        : spheres{std::move(spheres)}, cylinders{std::move(cylinders)}, pov{pov},
          samples_per_pixel{samples_per_pixel}, max_depth{max_depth},
          material_rng_seed{material_rng_seed}, rays_rng_seed{rays_rng_seed},
          background_dark_color{(background_dark_color)},
          background_light_color{background_light_color} { }

    // Método
    bool test_sphere_intersections(Ray & ray, double & closest_distance, Point & closest_point,
                                   Vector & closest_normal);
    bool test_cylinder_intersections(Ray & ray, double & closest_distance, Point & closest_point,
                                     Vector & closest_normal);
    void find_closest_intersection(Ray & ray);

    void add_sphere(Sphere const & sphere) { spheres.push_back(sphere); }

    void add_cylinder(Cylinder const & cylinder) { cylinders.push_back(cylinder); }

    void add_material_matte(Matte const & matte, std::string const & line_content) {
      if (material_index.contains(matte.get_name())) {
        parse::throw_material_exists(matte.get_name(), line_content);
      }

      // Añadimos matte en vector mattes
      std::size_t new_index            = mattes.size();
      material_index[matte.get_name()] = new_index * 10 + 0;
      mattes.push_back(matte);
    }

    void add_material_metal(Metal const & metal, std::string const & line_content) {
      if (material_index.contains(metal.get_name())) {
        parse::throw_material_exists(metal.get_name(), line_content);
      }

      // Añadimos matte en vectir mattes
      std::size_t new_index            = metals.size();
      material_index[metal.get_name()] = new_index * 10 + 0;
      metals.push_back(metal);
    }

    void add_material_refractive(Refractive const & refractive, std::string const & line_content) {
      if (material_index.contains(refractive.get_name())) {
        parse::throw_material_exists(refractive.get_name(), line_content);
      }

      // Añadimos matte en vectir mattes
      std::size_t new_index                 = refractives.size();
      material_index[refractive.get_name()] = new_index * 10 + 0;
      refractives.push_back(refractive);
    }

    [[nodiscard]] Pixel get_pixel_color(int f, int c);
    [[nodiscard]] Pov get_pov() const;
    [[nodiscard]] std::map<std::string, std::size_t> & get_material_index();

    [[nodiscard]] std::vector<render::Matte> const & get_mattes() const { return mattes; }

    [[nodiscard]] std::vector<render::Metal> const & get_metals() const { return metals; }

    [[nodiscard]] std::vector<render::Refractive> const & get_refractives() const {
      return refractives;
    }

  private:
    std::vector<Sphere> spheres;          // Vector de esferas
    std::vector<Cylinder> cylinders;      // Vector de cilindros
    render::Pov pov;                      // Cámara
    std::vector<Metal> metals;            // Vector de metales
    std::vector<Matte> mattes;            // Vector de mates
    std::vector<Refractive> refractives;  // Vector de refractivos
    std::map<std::string, std::size_t>
        material_index;  // Mapa para indexar materiales con nombre y flag

    int samples_per_pixel;         // Muestras por pixel
    int max_depth;                 // Profundidad maxima de rayos
    uint64_t material_rng_seed;    // Semilla para el generador de numeros aleatorios de materiales
    uint64_t rays_rng_seed;        // Semilla para el generador de numeros aleatorios de rayos
    Color background_dark_color;   // Color oscuro del fondo
    Color background_light_color;  // Color claro del fondo
    double gamma = 2.2;            // Valor de gamma

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
