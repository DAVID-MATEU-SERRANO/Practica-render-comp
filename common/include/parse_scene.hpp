#include <istream>
#include <string>
#include <string_view>

class Scene;  // Al parecer esto se llamna forward declaration y avisa al compilador que existe una
              // clase llamada Scene y que se definira en otro sitio (QUE LOCURA)

namespace parse {

  void parse_scene_stream(std::string const & path,
                          Scene & out);  // Lee fichero y carga datos en un objeto Scene

  void parse_scene_stream(std::string_view text,
                          Scene & out);  // Lee string y carga datos en un objeto Scene

  void parse_scene_stream(std::istream & in, Scene & out);

}  // namespace parse
