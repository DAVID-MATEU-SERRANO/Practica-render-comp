#ifndef RENDER_OBJECT_HPP
#define RENDER_OBJECT_HPP

#include <string>

namespace render {

  class object {
  public:
    object(std::string type, double cords_x, double cords_y, double cords_z, double radius);
    // [[nodiscard]] sirve para si haces operaciones y no se usan se eliminen
    [[nodiscard]] std::string get_type() const;
    [[nodiscard]] double get_cords_x() const;
    [[nodiscard]] double get_cords_y() const;
    [[nodiscard]] double get_cords_z() const;
    [[nodiscard]] double get_radius() const;

  private:
    std::string type_;
    double cords_x;
    double cords_y;
    double cords_z;
    double radius;
  };

}  // namespace render

#endif
