#include <istream>

class Scene;  // Al parecer esto se llamna forward declaration y avisa al compilador que existe una
              // clase llamada Scene y que se definira en otro sitio (QUE LOCURA)

namespace parse {

  void parse_scene_stream(std::istream & in, Scene & scene);

}  // namespace parse

// TODO: Valores por defecto en cada clase individual AAAAAAAAAAAAAAAAAa
