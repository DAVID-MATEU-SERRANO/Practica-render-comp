#include "parse_exception.hpp"
#include <gtest/gtest.h>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// Aquí vamos a declarar los mocks y stub necesarios para hacer las pruebas

namespace render {

  // STUB: Clases de datos mínimas para que el código compile
  class Point {
  public:
    Point(double, double, double) { }
  };

  class Vector {
  public:
    Vector(double, double, double) { }
  };

  class Pov {
  public:
  };

  class Ray {
  public:
  };

  // STUB/MOCK: Color debe simular la validación de rango del constructor.
  class Color {
  public:
    Color(double r, double g, double b) {
      // Simula la validación de rango: {0.0, 1.2, 0.0} fallaría
      if (r < 0 or r > 1 or g < 0 or g > 1 or b < 0 or b > 1) {
        throw std::runtime_error("Invalid color parameters");
      }
    }
  };

  // MOCK: Materiales y Objetos (Almacenarán los datos para verificación)
  // Usaremos un stub simple para los objetos geométricos, asumiendo que sus constructores son
  // válidos.
  class Matte {
  private:
    std::string name_;

  public:
    Matte(std::string name, Color) : name_(std::move(name)) { }

    [[nodiscard]] std::string get_name() const { return name_; }
  };

  class Metal {
  private:
    std::string name_;

  public:
    Metal(std::string name, Color, double) : name_(std::move(name)) { }

    [[nodiscard]] std::string get_name() const { return name_; }
  };

  class Refractive {
  private:
    std::string name_;

  public:
    Refractive(std::string name, double) : name_(std::move(name)) { }

    [[nodiscard]] std::string get_name() const { return name_; }
  };

  using t_material = std::variant<Matte, Metal, Refractive>;

  class Sphere {
  public:
    Sphere(Point, double radius, t_material const &) {
      if (radius <= 0.0) {
        throw std::runtime_error("Invalid sphere radius");
      }
    }
  };

  class Cylinder {
  public:
    Cylinder(Point, double radius, Vector, t_material const &) {
      if (radius <= 0.0) {
        throw std::runtime_error("Invalid cylinder radius");
      }
      // No validamos el vector nulo aquí para simplificar el mock.
    }
  };

  using t_material = std::variant<Matte, Metal, Refractive>;

  // MOCK: Clase Scene para verificar las llamadas (el estado)
  class Scene {
  public:
    std::map<std::string, size_t> material_index;
    std::vector<render::Matte> mattes;
    std::vector<render::Metal> metals;
    std::vector<render::Refractive> refractives;
    std::vector<render::Sphere> spheres;
    std::vector<render::Cylinder> cylinders;
    render::Pov pov;

    // Add functions to verify calls
    void add_material_matte(Matte const & m, std::string const & line_content) {
      // Mock de la lógica de indexación para que los tests de objetos puedan usarla
      if (material_index.contains(m.get_name())) {
        parse::throw_material_exists(m.get_name(), line_content);
      }
      material_index[m.get_name()] = mattes.size() * 10 + 0;
      mattes.push_back(m);
    }

    void add_sphere(Sphere const & s) { spheres.push_back(s); }

    void add_cylinder(Cylinder const & c) { cylinders.push_back(c); }

    void add_material_metal(Metal const & m, std::string const & line_content) {
      // Mock de la lógica de indexación para que los tests de objetos puedan usarla
      if (material_index.contains(m.get_name())) {
        parse::throw_material_exists(m.get_name(), line_content);
      }
      material_index[m.get_name()] = metals.size() * 10 + 1;
      metals.push_back(m);
    }

    void add_material_refractive(Refractive const & m, std::string const & line_content) {
      // Mock de la lógica de indexación para que los tests de objetos puedan usarla
      if (material_index.contains(m.get_name())) {
        parse::throw_material_exists(m.get_name(), line_content);
      }
      material_index[m.get_name()] = refractives.size() * 10 + 2;
      refractives.push_back(m);
    }
  };

  void dispatch_scene_entity(std::string const & tag, std::vector<std::string> const & tokens,
                             Scene & scene, std::string const & line_content);

}  // namespace render

namespace {

  // Inicialización de constantes
  std::string const TEST_LINE = "Test line content";

  // Prueba de los materiales

  // Caso de error función miembro: color parse Matte fuera de rango
  TEST(test_parser_materials, parse_matte_line_invalid_color_range_throws) {
    render::Scene scene;
    std::string const tag           = "matte";
    std::vector<std::string> tokens = {"mat2", "0.0", "1.2", "0.5"};

    EXPECT_THROW(
        { render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); }, parse::ParseException);
  }

  // Caso de error función miembro: falta factor difusión en metal
  TEST(test_parser_materials, dispatch_metal_line_insufficient_tokens_throws) {
    render::Scene scene;
    std::string const tag           = "metal";
    std::vector<std::string> tokens = {"met1", "0.5", "0.5", "0.5"};

    // Esperamos que util::expect_token_count falle.
    EXPECT_THROW(
        { render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); }, parse::ParseException);
  }

  // Caso de error función miembro: índice inválido
  TEST(test_parser_materials, dispatch_refractive_line_invalid_index_throws) {
    render::Scene scene;
    // Tags y Tokens para: refractive: name 1.0 (1.0 es inválido)
    std::string const tag           = "refractive";
    std::vector<std::string> tokens = {"ref_fail", "1.0"};

    // Esperamos que el constructor de Refractive falle y relance ParseException.
    EXPECT_THROW(
        { render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); }, parse::ParseException);
  }

  // Pruebas para los objetos

  // Caso de error función miembro: referencia a material no definido.
  TEST(test_parser_objects, dispatch_sphere_material_not_found_throws) {
    render::Scene scene;  // El índice está vacío
    std::string const tag           = "sphere";
    std::vector<std::string> tokens = {"1", "1", "1", "0.5", "missing_mat"};

    // Esperamos que lance throw_material_not_found.
    EXPECT_THROW(
        { render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); }, parse::ParseException);
  }

  // Caso de error función miembro: radio de esfera inválido (negativo o cero).
  TEST(test_parser_objects, dispatch_sphere_invalid_radius_throws) {
    render::Scene scene;
    scene.add_material_matte(render::Matte("m1", {1, 1, 1}), "");  // Añadir m1 (código 0)
    std::string const tag           = "sphere";
    std::vector<std::string> tokens = {"1", "1", "1", "-0.5", "m1"};

    EXPECT_THROW(
        { render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); }, parse::ParseException);
  }

  // Caso de error función miembro: vector de eje de cilindro nulo.
  TEST(test_parser_objects, dispatch_cylinder_zero_edge_throws) {
    render::Scene scene;
    scene.add_material_matte(render::Matte("m1", {1, 1, 1}), "");  // Añadir m1 (código 0)
    std::string const tag           = "cylinder";
    std::vector<std::string> tokens = {"0", "0", "0", "1.0", "0", "0", "0", "m1"};

    EXPECT_THROW(
        { render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); }, parse::ParseException);
  }

  // Caso de prueba: parsing y adición de la esfera.
  TEST(test_parser_objects, dispatch_valid_sphere_adds_object) {
    render::Scene scene;
    scene.add_material_matte(render::Matte("m1", {1, 1, 1}), "");
    std::string const tag           = "sphere";
    std::vector<std::string> tokens = {"0", "0", "0", "1.0", "m1"};

    EXPECT_NO_THROW({ render::dispatch_scene_entity(tag, tokens, scene, TEST_LINE); });

    EXPECT_EQ(scene.spheres.size(), 1);
  }

}  // namespace
