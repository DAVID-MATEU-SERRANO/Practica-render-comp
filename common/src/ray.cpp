#include "ray.hpp"
#include "sphere.hpp"
#include "vector.hpp"
#include <cmath>
#include <limits>

namespace render {

  Point const & Ray::get_origin() const {
    return origin;
  }

  Vector const & Ray::get_direction() const {
    return direction;
  }

  bool Ray::sphere_intersection(Sphere const & sphere) {
    double a = std::pow(direction.magnitude(), 2);
    double b = 2.0 * direction.dot(sphere.get_center().substract(origin));
    double c = std::pow(sphere.get_center().substract(origin).magnitude(), 2) -
               std::pow(sphere.get_radius(), 2);

    double discriminant = std::sqrt(b * b - 4 * a * c);
    if (discriminant < 0) {
      return false;
    }
    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);

    if (t1 < 0 and t2 < 0) {
      return false;
    }
    if (t1 >= 0 and t2 >= 0) {
      intersection_distance = std::min(t1, t2);
    } else if (t1 >= 0) {
      intersection_distance = t1;
    } else {
      intersection_distance = t2;
    }

    point_intersection = origin.add(direction.dot(intersection_distance));

    normal_vector = point_intersection.substract(sphere.get_center()).dot(1 / sphere.get_radius());
    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::cylinder_side_intersection(Cylinder const & cylinder) {
    Vector rc = origin.substract(cylinder.get_center());
    double a  = std::pow(direction.perpendicular_component(cylinder.get_edge()).magnitude(), 2);
    double b  = 2.0 * (rc.perpendicular_component(cylinder.get_edge())
                          .dot(direction.perpendicular_component(cylinder.get_edge())));
    double c  = std::pow(rc.perpendicular_component(cylinder.get_edge()).magnitude(), 2) -
               std::pow(cylinder.get_radius(), 2);

    double discriminant = std::sqrt(b * b - 4 * a * c);
    if (discriminant < 0) {
      return false;
    }
    double t1 = (-b - discriminant) / (2 * a);
    double t2 = (-b + discriminant) / (2 * a);
    if (t1 < 0 and t2 < 0) {
      return false;
    }
    if (t1 >= 0 and t2 >= 0) {
      intersection_distance = std::min(t1, t2);
    } else if (t1 >= 0) {
      intersection_distance = t1;
    } else {
      intersection_distance = t2;
    }

    point_intersection = origin.add(direction.dot(intersection_distance));
    if (point_intersection.substract(cylinder.get_center()).dot(cylinder.get_edge()) >
        (cylinder.get_height() / 2))
    {
      return false;
    }

    normal_vector = point_intersection.substract(cylinder.get_center())
                        .perpendicular_component(cylinder.get_edge());
    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::cylinder_upper_base_intersection(Cylinder const & cylinder) {
    Point p       = cylinder.get_center().add(cylinder.get_edge().dot(cylinder.get_height() / 2));
    normal_vector = cylinder.get_edge();
    Vector rp     = origin.substract(p);

    if (std::abs(direction.dot(normal_vector)) < 1e-8) {
      return false;
    }

    intersection_distance = rp.dot(normal_vector) / direction.dot(normal_vector);

    point_intersection = origin.add(direction.dot(intersection_distance));
    if (point_intersection.substract(p).magnitude() > cylinder.get_radius()) {
      return false;
    }

    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  bool Ray::cylinder_lower_base_intersection(Cylinder const & cylinder) {
    Point p = cylinder.get_center().substract(cylinder.get_edge().dot(cylinder.get_height() / 2));
    normal_vector = cylinder.get_edge().dot(-1);
    Vector rp     = origin.substract(p);

    if (std::abs(direction.dot(normal_vector)) < 1e-8) {
      return false;
    }

    intersection_distance = rp.dot(normal_vector) / direction.dot(normal_vector);

    point_intersection = origin.add(direction.dot(intersection_distance));
    if (point_intersection.substract(p).magnitude() > cylinder.get_radius()) {
      return false;
    }

    // Ensure the normal vector points against the ray direction
    if (normal_vector.dot(direction) > 0) {
      normal_vector = normal_vector.dot(-1);
    }

    return true;
  }

  void Ray::find_closest_intersection(Scene const & scene) {
    // Find the closest intersection with objects in the scene
  }

}  // namespace render
