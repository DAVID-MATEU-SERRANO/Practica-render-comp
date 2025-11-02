#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

// Asumimos que esta función de indexación representa la lógica en tu main.cpp
// Se define externamente para poder ser probada.
namespace render {

  std::size_t compute_soa_index(int f, int c, int image_width);

  // Definición de Pixel (para verificar el tipo de datos)
  struct Pixel {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
  };

}  // namespace render

namespace {  // Namespace anónimo

  int const TEST_WIDTH           = 100;
  int const TEST_HEIGHT          = 10;
  std::size_t const TOTAL_PIXELS = TEST_WIDTH * TEST_HEIGHT;

  // =========================================================================
  // 1. PRUEBAS DE INDEXACIÓN (LÓGICA CRÍTICA)
  // =========================================================================

  // Verifica que la función de indexación (Row-Major) devuelva el índice correcto.
  TEST(test_soa_index, correct_row_major_indexing) {
    int const WIDTH = 1'920;

    // Esquina superior izquierda (0, 0)
    EXPECT_EQ(render::compute_soa_index(0, 0, WIDTH), 0);

    // Mitad de la primera fila (0, 960)
    EXPECT_EQ(render::compute_soa_index(0, 960, WIDTH), 960);

    // Inicio de la segunda fila (1, 0) - Debe ser 1 * 1920
    EXPECT_EQ(render::compute_soa_index(1, 0, WIDTH), 1'920);

    // Píxel central (10, 50)
    EXPECT_EQ(render::compute_soa_index(10, 50, WIDTH), 10 * 1'920 + 50);
  }

  // Verifica que el último píxel (esquina inferior derecha) se indexe correctamente.
  TEST(test_soa_index, last_pixel_index_is_correct) {
    int const W = 10;
    int const H = 5;  // Total de 50 píxeles, índices de 0 a 49.

    // Fila 4, Columna 9
    EXPECT_EQ(render::compute_soa_index(H - 1, W - 1, W), 49);
  }

  // =========================================================================
  // 2. PRUEBAS DE REPRESENTACIÓN EN MEMORIA (SOA)
  // =========================================================================

  // Verifica que la estructura de arrays (R, G, B separados) funcione y se acceda correctamente.
  TEST(test_soa_storage, soa_representation_stores_data_in_separate_arrays) {
    std::vector<uint8_t> R(TOTAL_PIXELS);
    std::vector<uint8_t> G(TOTAL_PIXELS);
    std::vector<uint8_t> B(TOTAL_PIXELS);

    std::size_t const index_red   = 10;
    std::size_t const index_green = 500;

    // Almacenamiento
    R[index_red] = 255;
    G[index_red] = 10;  // G en el mismo índice que R
    B[index_red] = 0;

    R[index_green] = 0;
    G[index_green] = 200;  // G en un índice diferente
    B[index_green] = 0;

    // Verificación de Acceso (SOA: los canales son independientes pero indexados igual)
    EXPECT_EQ(R[index_red], 255);
    EXPECT_EQ(G[index_red], 10);

    // Verificación de localidad (El valor de G en el índice 500 es diferente del índice 10)
    EXPECT_EQ(G[index_green], 200);
  }

}  // namespace
