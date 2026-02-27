#pragma once

#include "core/Vec3.h"
#include "core/HitRecord.h"
#include "materials/Material.h"
#include "util/RNG.h"

/**
 * BSDF (Bidirectional Scattering Distribution Function)
 * Models how light scatters at a surface — reflection, transmission, or both.
 *
 * Responsibilities:
 *   - Select which lobe to sample (diffuse, specular, transmission)
 *   - Generate a new direction (wi)
 *   - Evaluate the scattering function (f) and pdf
 */
struct BSDFSample {
    Vec3 wi;   // Sampled incoming direction towards next bounce
    Color f;   // BSDF value f(wo, wi)
    float pdf; // Probability of sampling wi
};

Color BSDF_Eval(
    const Material& material,
    const HitRecord& record,
    const Vec3& w0,
    const Vec3& wi
);

float BSDF_Pdf(
    const Material& material,
    const HitRecord& record,
    const Vec3& w0,
    const Vec3& wi
);

/**
 * Compute how light scatters when hitting a material.
 * 
 * @param material Material to scatter light off of
 * @param record Surface intersection details (position, normal, etc.)
 * @param wo Outgoing vector from surface toward previous bounce/camera
 * @param rng Random number generator
 * @return BSDFSample containing new direction, BRDF, and pdf values
 */
BSDFSample BSDF_Sample(
    const Material& material,
    const HitRecord& record,
    const Vec3& wo,
    RNG& rng
);