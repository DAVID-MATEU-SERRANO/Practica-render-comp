#include "../include/render.hpp"
#include "../include/parse_config.hpp"
#include "../include/parse_scene.hpp"
#include "../include/scene.hpp"
#include <fstream>

namespace render {

  bool load_scene_from_files(render::Scene & scene, std::ifstream & in, std::ifstream & file) {
    parse::parse_scene_stream(in, scene);

    parse::parse_config_stream(file, scene);
    return true;
  }

}  // namespace render
