#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Pixel {
  int r, g, b;
};

bool leerPPM(std::string const & filename, int & width, int & height, std::vector<Pixel> & data) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error al abrir el archivo: " << filename << std::endl;
    return false;
  }

  std::string line;
  std::getline(file, line);

  if (line != "P3") {
    std::cerr << "Formato no soportado (debe ser P3): " << line << std::endl;
    return false;
  }

  // Ignorar comentarios
  while (std::getline(file, line)) {
    if (line[0] != '#') {
      break;
    }
  }

  std::istringstream dimensions(line);
  dimensions >> width >> height;

  int max_val;
  file >> max_val;

  data.resize(width * height);
  for (int i = 0; i < width * height; ++i) {
    file >> data[i].r >> data[i].g >> data[i].b;
  }

  return true;
}

double diferenciaPixel(Pixel const & a, Pixel const & b) {
  return (std::abs(a.r - b.r) + std::abs(a.g - b.g) + std::abs(a.b - b.b)) / 3.0;
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " imagen1.ppm imagen2.ppm" << std::endl;
    return 1;
  }

  std::string file1 = argv[1];
  std::string file2 = argv[2];

  int width1, height1, width2, height2;
  std::vector<Pixel> img1, img2;

  if (!leerPPM(file1, width1, height1, img1)) {
    return 1;
  }
  if (!leerPPM(file2, width2, height2, img2)) {
    return 1;
  }

  if (width1 != width2 || height1 != height2) {
    std::cerr << "Las imágenes tienen diferente tamaño" << std::endl;
    return 1;
  }

  double max_diff         = 0.0;
  double sum_squared_diff = 0.0;

  for (size_t i = 0; i < img1.size(); ++i) {
    double diff = diferenciaPixel(img1[i], img2[i]);
    if (diff > max_diff) {
      max_diff = diff;
    }
    sum_squared_diff += diff * diff;
  }

  double mse = std::sqrt(sum_squared_diff / img1.size());

  std::cout << "Diferencia máxima: " << max_diff << std::endl;
  std::cout << "Error cuadrático medio (MSE): " << mse << std::endl;

  if (max_diff < 150 && mse < 10) {
    std::cout << "Resultado: aceptable" << std::endl;
  } else {
    std::cout << "Resultado: NO aceptable" << std::endl;
  }

  return 0;
}
