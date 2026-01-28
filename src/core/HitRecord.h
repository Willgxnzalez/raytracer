#pragma once
#include "../math/Vec3.h"
#include "Ray.h"
#include <cassert>

/**
 * HitRecord - A data packet receipt that describes a ray-surface intersection and 
 * only contains what renderer needs after hit.
 * Key invariant: surface normal (normalized) always points against incoming ray direction
 */
struct HitRecord {
    Vec3 position;
    Vec3 surfaceNormal;
    double t;
    bool frontFace;

    inline void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
        assert(std::abs(outwardNormal.lengthSquared() - 1.0) < 1e-6);

        frontFace = dot(ray.direction, outwardNormal) < 0;
        surfaceNormal = frontFace ? outwardNormal : -outwardNormal;
    }
};

