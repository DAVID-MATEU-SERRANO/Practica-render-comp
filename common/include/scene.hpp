// Estructura de datos para almacenar la informacion de una escena
// Contiene los parametros que se obtienen de leer el archivo scene.txt

#include <array>
#include <cstddef>
#include <sstream>    // para std::ostringstream
#include <stdexcept>  // para lanzar errores
#include <string>
#include <unordered_map>  // para poder usar diccionarios
#include <vector>

// Estructura para almacenar la informacion de un material

struct Material {
  std::string type;  // Tipo de material: matte, metal, refractive
  std::string name;  // Nombre del material

  // Propiedades del material
  std::array<double, 3> color{0.0, 0.0, 0.0};  // Color RGB (para matte y metal)
  double roughness        = 0.0;               // Rugosidad (para metal)
  double refractive_index = 1.0;               // Indice de refraccion (para refractive)
};

// Estructura para almacenar la informacion de un objeto

struct Object {
  std::string type;           // Tipo de objeto: sphere, cylinder
  std::string material_name;  // Nombre del material asociado al objeto
};

// Estructura para almacenar la informacion de una esfera

struct Sphere : public Object {
  std::array<double, 3> center{0.0, 0.0, 0.0};  // Centro de la esfera (x, y, z)
  double radius = 1.0;                          // Radio de la esfera
};

// Estructura para almacenar la informacion de un cilindro

struct Cylinder : public Object {
  std::array<double, 3> base{0.0, 0.0, 0.0};  // Punto 1 del eje del cilindro (x, y, z)
  std::array<double, 3> axis{0.0, 0.0, 0.0};  // Punto 2 del eje del cilindro (x, y, z)
  double radius = 1.0;                        // Radio del cilindro
};

// Estructura para almacenar la informacion de la escena completa

struct Scene {
  std::vector<Material> materials;  // Vector de materiales
  std::vector<Sphere> spheres;      // Vector de esferas
  std::vector<Cylinder> cylinders;  // Vector de cilindros

  // Mapa para buscar materiales por su nombre
  std::unordered_map<std::string, size_t> material_index;

  // FUNCIONES PARA AGREGAR MATERIALES Y OBJETOS A LA ESCENA
  // Agregar material a la escena (verificar que no exista otro material con el mismo nombre)
  void add_material(Material const & m, std::string const & lineforprint) {
    if (material_index.find(m.name) != material_index.end()) {
      std::ostringstream oss;
      oss << "Material with name '[" << m.name << "]' already exists\n"
          << "Line: " << lineforprint;
      throw std::runtime_error(oss.str());
    }
    material_index[m.name] = materials.size();
    materials.push_back(m);
  }

  void add_sphere(Sphere const & sphere) { spheres.push_back(sphere); }

  void add_cylinder(Cylinder const & cylinder) { cylinders.push_back(cylinder); }

  void clear() {
    materials.clear();
    spheres.clear();
    cylinders.clear();
    material_index.clear();
  }
};
