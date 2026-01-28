#pragma once
#include "Vec3.h"

/**
 * Ray - Half-line defined by origin and direction.
 * Formula: P(t) = origin + t * direction, where t >= 0.
 */
struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray() = default;
    Ray(const Vec3& _origin, const Vec3& _direction) : origin(_origin), direction(_direction) {}

    Vec3 at(double t) const{
        return origin + t * direction;
    }
};