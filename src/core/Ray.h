#pragma once
#include "core/Vec3.h"

/**
 * Ray - Half-line defined by origin and direction.
 * Formula: P(t) = origin + t * direction, where t >= 0.
 */
struct Ray {
    Point3 origin;
    Vec3 direction;

    Ray() : origin(Point3{0, 0, 0}), direction(Vec3{0, 0, 0}) {}
    Ray(const Point3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    Point3 at(float t) const{
        return origin + t * direction;
    }
};