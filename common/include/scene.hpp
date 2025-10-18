// Estructura de datos para almacenar la informacion de una escena
// Contiene los parametros que se obtienen de leer el archivo scene.txt

//============================= EJEMPLO DE ARCHIVO scene.txt =============================
/*
# ======= MATERIALES =======
matte: rojo 1.0 0.1 0.1
metal: acero 0.8 0.8 0.8 0.2
refractive: vidrio 1.5

# ======= OBJETOS =======
sphere: 0.0 0.0 -1.0 0.5 rojo
sphere: 1.0 0.0 -1.5 0.5 acero
sphere: -1.0 0.0 -1.5 0.5 vidrio

cylinder: 0.0 -0.5 -1.0 0.0 1.0 0.0 0.3 acero
*/

//==========================================================================================

#include <array>
#include <cstddef>
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
  void add_material(Material const & m) {
    if (material_index.contains(m.name)) {
      throw std::runtime_error("Error: Material with name '" + m.name + "' already exists.");
    }
    material_index[m.name] = materials.size();
    materials.push_back(m);
  }

  // Buscar material por su nombre
  Material const & get_material(std::string const & name) const {
    auto it = material_index.find(name);
    if (it == material_index.end()) {
      throw std::runtime_error("Error: Material with name '" + name + "' not found");
    }
    return materials[it->second];
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
