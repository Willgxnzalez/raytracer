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
