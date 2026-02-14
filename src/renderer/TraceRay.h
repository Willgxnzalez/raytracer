#pragma once

#include "core/Ray.h"
#include "core/Vec3.h"
#include "core/HitRecord.h"
#include "materials/Material.h"
#include "util/RNG.h"
#include <cmath>

/**
 * Compute the color of a ray by iteratively tracing it through the scene, accumulating color from surface interactions
 * and material scattering through multiplicative attenuation until hitting the background or reaching max depth.
 * 
 * @param ray Initial ray to trace
 * @param scene World containing all hittable objects
 * @param RNG Random number generator
 * @param maxDepth Maximum number of bounces allowed
 * @return Final color accumulated along the ray path
 */
Color traceRay(const Ray& ray, const Scene& scene, RNG& rng, int maxDepth) {
    Ray currentRay = ray;
    Color attenuation(1.0f, 1.0f, 1.0f); // Start with full intensity white light
    float EPS = 1e-4f; // prevent self intersections

    for (int depth = 0; depth < maxDepth; ++depth) {
        HitRecord rec;
        if (scene.intersect(rec, currentRay, EPS, INFINITY)) {
            Ray scattered;
            Color materialAttenuation;
            
            if (rec.material->scatter(rec, currentRay, scattered, materialAttenuation, rng)) {
                attenuation *= materialAttenuation;
                currentRay = scattered;
            } else {
                return Color(0.0f, 0.0f, 0.0f); // Ray absorbed - return black
            }
        } else {
            // Hit background - compute and return final color
            Vec3 unitDir = currentRay.direction.normalized();
            float t = 0.5 * (unitDir.y + 1.0);
            Color backgroundColor = (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
            return attenuation * backgroundColor;
        }
    }
    return Color(0.0f, 0.0f, 0.0f); // Max depth reached - return black (absorbed by bounces)
}