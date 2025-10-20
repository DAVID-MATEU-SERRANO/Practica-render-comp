// Estructura de datos para almacenar la informacion de una escena
// Contiene los parametros que se obtienen de leer el archivo scene.txt

#include "cylinder.hpp"
#include "matte.hpp"
#include "metal.hpp"
#include "refractive.hpp"
#include "sphere.hpp"
#include <cstddef>
#include <sstream>    // para std::ostringstream
#include <stdexcept>  // para lanzar errores
#include <string>
#include <unordered_map>  // para poder usar diccionarios
#include <vector>

struct Scene {
  std::vector<render::Matte> mattes;             // Vector Matte
  std::vector<render::Metal> metales;            // Vector Metal
  std::vector<render::Refractive> refractarios;  // Vector Refractive
  std::vector<render::Sphere> spheres;           // Vector de esferas
  std::vector<render::Cylinder> cylinders;       // Vector de cilindros

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
  }
};
