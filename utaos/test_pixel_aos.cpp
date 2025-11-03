#include "../../common/include/scene.hpp"
#include "../aos/include/image_aos.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(pixel_aos, constructs_with_given_size) {
  PixelAOS p0(0);
  EXPECT_EQ(p0.pixels.size(), 0U);

  PixelAOS p5(5);
  EXPECT_EQ(p5.pixels.size(), 5U);
}

TEST(pixel_aos, set_writes_pixel_at_index) {
  PixelAOS p(3);
  Pixel red{255, 0, 0};
  Pixel green{0, 255, 0};
  Pixel blue{0, 0, 255};

  p.set(0, red);
  p.set(1, green);
  p.set(2, blue);

  ASSERT_EQ(p.pixels.size(), 3U);
  EXPECT_EQ(p.pixels[0].r, 255);
  EXPECT_EQ(p.pixels[0].g, 0);
  EXPECT_EQ(p.pixels[0].b, 0);

  EXPECT_EQ(p.pixels[1].r, 0);
  EXPECT_EQ(p.pixels[1].g, 255);
  EXPECT_EQ(p.pixels[1].b, 0);

  EXPECT_EQ(p.pixels[2].r, 0);
  EXPECT_EQ(p.pixels[2].g, 0);
  EXPECT_EQ(p.pixels[2].b, 255);
}

TEST(pixel_aos, row_major_indexing_matches_main_formula) {
  // Simula el indexing que usa tu main (f * image_width + c).
  int const width         = 3;
  int const height        = 2;
  std::size_t const total = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
  PixelAOS p(total);

  // Marcamos cada píxel con un color que codifica su fila/columna para verificar indexado
  for (int f = 0; f < height; ++f) {
    for (int c = 0; c < width; ++c) {
      std::size_t idx = static_cast<std::size_t>(f) * static_cast<std::size_t>(width) +
                        static_cast<std::size_t>(c);
      Pixel pix{
        static_cast<unsigned char>(10 * f),  // r = 0,10,20...
        static_cast<unsigned char>(20 * c),  // g = 0,20,40...
        static_cast<unsigned char>(255)      // b fijo
      };
      p.set(idx, pix);
    }
  }

  // Verifica un par de posiciones clave
  // (f=0,c=0) -> idx=0
  EXPECT_EQ(p.pixels[0].r, 0);
  EXPECT_EQ(p.pixels[0].g, 0);
  EXPECT_EQ(p.pixels[0].b, 255);

  // (f=0,c=2) -> idx=2
  EXPECT_EQ(p.pixels[2].r, 0);
  EXPECT_EQ(p.pixels[2].g, 40);
  EXPECT_EQ(p.pixels[2].b, 255);

  // (f=1,c=0) -> idx=3
  EXPECT_EQ(p.pixels[3].r, 10);
  EXPECT_EQ(p.pixels[3].g, 0);
  EXPECT_EQ(p.pixels[3].b, 255);

  // (f=1,c=2) -> idx=5
  EXPECT_EQ(p.pixels[5].r, 10);
  EXPECT_EQ(p.pixels[5].g, 40);
  EXPECT_EQ(p.pixels[5].b, 255);
}
