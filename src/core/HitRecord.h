#pragma once
#include "core/Vec3.h"
#include <cassert>

struct Material; // forward declare

/**
 * HitRecord - A data packet receipt that describes a ray-surface intersection and 
 * only contains what renderer needs after hit.
 * Key invariant: surface normal (normalized) always points against incoming ray direction
 */
struct HitRecord {
    Vec3 position;
    Vec3 normal; // outward surface normal
    float t;
    bool frontFace;
    const Material* material;

    inline void setFaceNormal(const Vec3& rayDirection, const Vec3& outwardNormal) {
        assert(std::abs(outwardNormal.lengthSquared() - 1.0) < 1e-6);

        frontFace = dot(rayDirection, outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

