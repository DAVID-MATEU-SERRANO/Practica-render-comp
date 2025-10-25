#include "render.hpp"
#include "parse_config.hpp"
#include "parse_scene.hpp"
#include <fstream>
#include <iostream>

namespace render {

  bool load_scene_from_files(render::Scene & scene, std::ifstream & in, std::ifstream & file) {
    parse::parse_scene_stream(in, scene);

    parse::parse_config_stream(file, scene);
    return true;
  }

}  // namespace render
