#pragma once
#include "../math/Vec3.h"

/**
 * Ray - Half-line defined by origin and direction.
 * Formula: P(t) = origin + t * direction, where t >= 0.
 */
struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray() : origin(Vec3{0, 0, 0}), direction(Vec3{0, 0, 0}) {}
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    Vec3 at(double t) const{
        return origin + t * direction;
    }
};