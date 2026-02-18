#pragma once

#include "materials/Material.h"
#include "core/Vec3.h"
#include "core/Ray.h"
#include "core/HitRecord.h"
#include "util/RNG.h"

// Represents the result of sampling a Bidirectional Scattering Distribution Function (BSDF),
// containing all the information needed to evaluate a single light bounce at a surface interaction.
struct BSDFSample {
    Vec3 direction; // Sampled direction toward next bounce from surface
    Color brdf;     // How light scatters on material surface
    float pdf;      // Probability of choosing sampled direction
};

// Orthonormal Basis (ONB) - a local coordinate frame aligned to a surface normal.
// Transform sampled directions from local space to world space,
// where w is the surface normal, and u/v are perpendicular tangent vectors.
struct ONB {
    Vec3 u, v, w;

    ONB(const Vec3& normal) {
        w = normal;
        // Choose an arbitrary vector not parallel to w to build the basis
        Vec3 a = (std::abs(w.x) > 0.9f) ? Vec3(0,1,0) : Vec3(1,0,0);
        v = cross(w, a).normalized();
        u = cross(w, v);
    }

    // Transforms a direction from local (ONB) space to world space
    Vec3 toWorld(const Vec3& local) const {
        return local.x * u + local.y * v + local.z * w;
    }
};

/**
 * Compute how light scatters when hitting this material.
 * 
 * @param material Material to scatter light off of
 * @param record Surface intersection details (position, normal, etc.)
 * @param wo Outgoing vector from surface toward previous bounce/camera
 * @param rng Random number generator
 * @return BSDFSample containing new direction, BRDF, and pdf values
 */
BSDFSample sampleMaterial(
    const Material& material,
    const HitRecord& rec,
    const Vec3& wo,
    RNG& rng
);
