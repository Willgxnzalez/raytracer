#pragma once
#include <cmath>
#include "../geometry/Hittable.h"
#include "../materials/Material.h"
#include "../math/Ray.h"
#include "../math/Vec3.h"
#include "../core/HitRecord.h"

/**
 * Compute the color of a ray by iteratively tracing it through the scene, accumulating color from surface interactions
 * and material scattering through multiplicative attenuation until hitting the background or reaching max depth.
 * 
 * @param ray Initial ray to trace
 * @param scene World containing all hittable objects
 * @param maxDepth Maximum number of bounces allowed
 * @return Final color accumulated along the ray path
 */
Vec3 rayColor(const Ray& ray, const Hittable& scene, int maxDepth) {
    Ray currentRay = ray;
    Vec3 attenuation(1, 1, 1); // Start with full intensity white light

    for (int depth = 0; depth < maxDepth; ++depth) {
        HitRecord rec;
        if (scene.hit(rec, currentRay, 0.001, INFINITY)) {
            Ray scattered;
            Vec3 materialAttenuation;
            
            if (rec.material->scatter(rec, currentRay, scattered, materialAttenuation)) {
                attenuation *= materialAttenuation;
                currentRay = scattered;
            } else {
                return Vec3(0, 0, 0); // Ray absorbed - return black
            }
        } else {
            // Hit background - compute and return final color
            Vec3 unitDir = currentRay.direction.normalized();
            double t = 0.5 * (unitDir.y + 1.0);
            Vec3 backgroundColor = (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
            return attenuation * backgroundColor;
        }
    }
    return Vec3(0, 0, 0); // Max depth reached - return black (absorbed by bounces)
}