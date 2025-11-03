#ifndef RENDER_LOGIC_HPP
#define RENDER_LOGIC_HPP

#include "../../common/include/render.hpp"
#include "../../common/include/scene.hpp"
#include <fstream>
#include <string>
#include <vector>

namespace render {

  void validate_arguments(int argc, std::vector<std::string> const & arguments);

  Scene load_scene(std::string const & config_path, std::string const & scene_path);

  void write_ppm_header(std::ofstream & file, int width, int height);

}  // namespace render

#endif
