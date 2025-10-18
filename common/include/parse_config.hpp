#include <istream>
#include <string>

class Config;  // forward declaration

namespace parse2 {

  void parse_config_stream(std::istream & in, Config & cfg);

  void parse_config_file(std::string const & filename, Config & cfg);

}  // namespace parse2
