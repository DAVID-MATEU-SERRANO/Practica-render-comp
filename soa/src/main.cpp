#include "../../common/include/config.hpp"
#include "../../common/include/parse_config.hpp"
#include "../../common/include/parse_scene.hpp"
#include "../../common/include/scene.hpp"
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char * argv[]) {
  // ####### CHECK PARAMETERS #######
  std::vector<std::string> args(argv + 1, argv + argc);
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n";
    return 1;
  }

  // ####### PARSE #######
  std::string const & scene_path  = args[1];
  std::string const & config_path = args[0];

  // Scene
  Scene scene{};
  std::ifstream scene_file(scene_path);
  if (!scene_file.is_open()) {
    std::cerr << "ERROR: Cannot open scene file: " << scene_path << '\n';
    return 1;
  }
  parse::parse_scene_stream(scene_file, scene);
  scene_file.close();

  // Config
  Config config{};
  std::ifstream config_file(config_path);
  if (!config_file.is_open()) {
    std::cerr << "ERROR: Cannot open scene file: " << config_path << '\n';
    return 1;
  }
  parse2::parse_config_stream(config_file, config);
  config_file.close();

  return 0;
}
