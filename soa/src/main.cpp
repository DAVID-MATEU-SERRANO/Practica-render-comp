#include "../../common/include/parse_scene.hpp"
#include "../../common/include/scene.hpp"
#include <iostream>
#include <vector>

int main(int argc, char * argv[]) {
  // CHECK PARAMETERS
  std::vector<std::string> args(argv + 1, argv + argc);
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n";
    return 1;
  }

  // PARSE
  std::string const & scene_path = args[2];
  // std::string const & config_path = args[1];

  // Scene
  Scene Scene{};
  parse::parse_scene_stream(scene_path, Scene);
  std::cout << Scene.materials[0].name << "\n";
}
