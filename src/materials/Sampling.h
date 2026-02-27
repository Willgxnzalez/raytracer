#pragma once

#include "core/Vec3.h"
#include "util/RNG.h"

// Orthonormal Basis (ONB) - a local coordinate frame aligned to a surface normal.
// Transform sampled directions from local space to world space,
// where w is the surface normal, and u/v are perpendicular tangent vectors.
struct ONB {
    Vec3 u, v, w;

    ONB(const Vec3& normal) {
        w = normal;
        Vec3 a = (std::abs(w.x) > 0.9f) ? Vec3(0,1,0) : Vec3(1,0,0);
        v = cross(w, a).normalized();
        u = cross(w, v);
    }

    // Transforms a direction from local (ONB) space to world space
    Vec3 toWorld(const Vec3& local) const {
        return local.x * u + local.y * v + local.z * w;
    }
};

Vec3 sampleCosineHemisphere(RNG& rng);
Vec3 randomInUnitSphere(RNG& rng);

Vec3 reflect(const Vec3& dir, const Vec3& normal);
Vec3 refract(const Vec3& dir, const Vec3& normal, float eta);

Vec3 sampleGGX(const Vec3& N, float alpha, RNG& rng);