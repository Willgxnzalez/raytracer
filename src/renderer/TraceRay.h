#pragma once

#include "core/Ray.h"
#include "core/Vec3.h"
#include "core/HitRecord.h"
#include "materials/BSDF.h"
#include "util/RNG.h"
#include <cmath>

/**
 * traceRay - Iterative Monte Carlo path tracing 
 * 
 * Computes the color of a ray by tracing it through the scene, accumulating color from surface interactions
 * and material scattering until hitting the background or reaching max depth. 
 * Rendering is probability-weighted energy transport.
 * 
 * @param ray Initial ray to trace
 * @param scene World containing all hittable objects
 * @param RNG Random number generator
 * @param maxDepth Maximum number of bounces allowed
 * @return Final color accumulated along the ray path
 */
inline Color traceRay(const Ray& ray, const Scene& scene, RNG& rng, int maxDepth) {
    Ray current = ray;
    Color throughput(1.0f, 1.0f, 1.0f); // Start with full intensity white light
    float SHADOW_EPS = 1e-2f; // prevent self intersections

    const auto& materials = scene.getMaterials();
    for (int depth = 0; depth < maxDepth; ++depth) {
        HitRecord record;
        if (scene.intersect(record, current, SHADOW_EPS, INFINITY)) {
            const Material& material = materials[record.materialIndex];
            if (material.type == MaterialType::Emissive) {
                return throughput * material.emission;
            }

            BSDFSample sample = BSDF_Sample(material, record, -current.direction, rng);
            if (sample.pdf <= 0.0f) break;

            float cosTheta = std::abs(dot(record.normal, sample.wi));

            if (material.type == MaterialType::Dielectric)
                throughput *= sample.f; // just Color(1) — no cos, no pdf division
            else {
                if (cosTheta <= 0.0f) break;
                throughput *= sample.f * cosTheta / sample.pdf;
            }

            current = Ray{record.position, sample.wi};
        } else {
            // Hit background - compute and return final color
            float t = 0.5 * (current.direction.y + 1.0); // Map [-1, 1] to [0, 1]
            Color backgroundColor = lerp(Vec3(1.0, 1.0, 1.0), Vec3(0.5, 0.7, 1.0), t);
            return throughput * backgroundColor;
        }
    }
    return Color(0.0f, 0.0f, 0.0f); // Max depth reached - return black (absorbed by bounces)
}