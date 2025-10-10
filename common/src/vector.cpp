#include "vector.hpp"

#include <cmath>

namespace render {

  double Vector::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  //...

}  // namespace render
