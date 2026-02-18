#pragma once

#include "materials/Material.h"
#include "core/Vec3.h"
#include "core/HitRecord.h"
#include "util/RNG.h"
#include <cmath>
#include <numbers>

// Represents the result of sampling a Bidirectional Scattering Distribution Function (BSDF),
// containing all the information needed to evaluate a single light bounce at a surface interaction.
struct BSDFSample {
    Vec3 wi; // Sampled direction toward next bounce from surface
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

inline Vec3 randomInUnitSphere(RNG& rng) {
    while(true) {
        // Unit cube point
        Vec3 v{
            rng.uniform(-1.0f, 1.0f), 
            rng.uniform(-1.0f, 1.0f), 
            rng.uniform(-1.0f, 1.0f)
        };
        if (v.lengthSquared() < 1.0f) return v; // check if cube point inside unit sphere
    }
}

inline Vec3 randomUnitVector(RNG& rng) {
    return randomInUnitSphere(rng).normalized();
}

inline Vec3 sampleCosineHemisphere(RNG& rng) {
    float r1 = rng.uniform01();
    float r2 = rng.uniform01();

    float phi = 2.0f * std::numbers::pi * r1;
    float x = std::cos(phi) * std::sqrt(r2);
    float y = std::sin(phi) * std::sqrt(r2);
    float z = std::sqrt(1.0f - r2);

    return Vec3(x, y, z);
}

inline Vec3 reflect(const Vec3& dir, const Vec3& normal) {
    return dir - 2.0f * dot(dir, normal) * normal;
}

inline Vec3 refract(const Vec3& dir, const Vec3& normal, float iorRatio) {
    float cosTheta = std::min(dot(-dir, normal), 1.0f);
    Vec3 perp = iorRatio * (dir + cosTheta * normal); // Perpendicular component of refracted ray
    Vec3 parallel = -std::sqrt(1.0f - perp.lengthSquared()) * normal; // Parallel component of refracted ray
    return perp + parallel;
}

/**
 * Compute how light scatters when hitting a material.
 * 
 * @param material Material to scatter light off of
 * @param record Surface intersection details (position, normal, etc.)
 * @param wo Outgoing vector from surface toward previous bounce/camera
 * @param rng Random number generator
 * @return BSDFSample containing new direction, BRDF, and pdf values
 */
inline BSDFSample sampleMaterial(
    const Material& material,
    const HitRecord& record,
    const Vec3& wo,
    RNG& rng
) {
    BSDFSample sample;
    switch (material.type) {
        case MaterialType::Diffuse: {
            // Pure Lambertian reflectance with cosine-weighted hemisphere sampling.
            // BRDF = albedo / pi  (Lambertian reflectance)
            // pdf  = cos(theta) / pi  (cosine-weighted hemisphere)
            // The cos(theta) terms cancel in the Monte Carlo estimator: (BRDF * cos(theta)) / pdf = albedo
            Vec3 localDir = sampleCosineHemisphere(rng);
            ONB onb{record.normal};

            sample.wi = onb.toWorld(localDir);
            sample.brdf = material.color / std::numbers::pi;
            sample.pdf = dot(record.normal, sample.wi) / std::numbers::pi;
            return sample;
        }

        // case MaterialType::Metal: {
        //     // Roughness simulates microscopic surface imperfections that scatter
        //     // reflected light into a cone around the perfect reflection direction.
        //     Vec3 reflected = reflect(rayIn.direction, record.normal);
        //     Vec3 scatteredDirection = reflected + material.roughness * randomInUnitSphere(rng); 
        //     rayOut = Ray{record.position, scatteredDirection};
        //     attenuation = material.color;
        //     return dot(scatteredDirection, record.normal) > 0;
        // }

        // case MaterialType::Dielectric: {
        //     float iorRatio = record.frontFace ? (1.0f / material.ior) : material.ior; // Ratio of indices of refraction depending on ray direction
        //     float cosTheta = std::min(dot(-rayIn.direction, record.normal), 1.0f);
        //     float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

        //     bool cannotRefract = iorRatio * sinTheta > 1.0f; // Check for total internal reflection

        //     // Schlick approximation for reflectance
        //     float R0 = (1.0f - material.ior) / (1.0f + material.ior);
        //     R0 = R0 * R0;
        //     float R = R0 + (1.0f - R0) * std::pow(1.0f - cosTheta, 5.0f);

        //     Vec3 scatteredDirection = (cannotRefract || rng.uniform01() < R) ?
        //         reflect(rayIn.direction, record.normal) : 
        //         refract(rayIn.direction, record.normal, iorRatio);
        
        //     rayOut = Ray{record.position, scatteredDirection};
        //     attenuation = Color(1.0f, 1.0f, 1.0f); // Dielectrics do not absorb light
        //     return true; // Always scatters light
        // }
        default: {
            sample.pdf = 0.0f;
            return sample;
        }
    }
}
