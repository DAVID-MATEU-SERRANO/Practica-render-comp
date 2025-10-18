#include <istream>
#include <string>

class Config;  // forward declaration

void parse_config_stream(std::istream & in, Config & cfg);

void parse_config_file(std::string const & filename, Config & cfg);
