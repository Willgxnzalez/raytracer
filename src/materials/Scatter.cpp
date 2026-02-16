#include "Scatter.h"

Vec3 randomInUnitSphere(RNG& rng) {
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

Vec3 randomUnitVector(RNG& rng) {
    return randomInUnitSphere(rng).normalized();
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

bool scatter(
    const Material& material,
    const HitRecord& record,
    const Ray& rayIn,
    Ray& rayOut,
    Vec3& attenuation,
    RNG& rng
) {
    switch (material.type) {
        case MaterialType::Diffuse: { // Pure diffuse - cosine-weighted hemisphere sampling
            Vec3 scatteredDirection = record.normal + randomUnitVector(rng);
            rayOut = Ray{record.position, scatteredDirection};
            attenuation = material.color;
            return true;
        }

        case MaterialType::Dielectric: {
            float iorRatio = record.frontFace ? (1.0f / material.ior) : material.ior; // Ratio of indices of refraction depending on ray direction
            float cosTheta = std::min(dot(-rayIn.direction, record.normal), 1.0f);
            float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

            bool cannotRefract = iorRatio * sinTheta > 1.0f; // Check for total internal reflection

            // Schlick approximation for reflectance
            float R0 = (1.0f - material.ior) / (1.0f + material.ior);
            R0 = R0 * R0;
            float R = R0 + (1.0f - R0) * std::pow(1.0f - cosTheta, 5.0f);

            Vec3 scatteredDirection = (cannotRefract || rng.uniform01() < R) ?
                reflect(rayIn.direction, record.normal) : 
                refract(rayIn.direction, record.normal, iorRatio);
        
            rayOut = Ray{record.position, scatteredDirection};
            attenuation = Color(1.0f, 1.0f, 1.0f); // Dielectrics do not absorb light
            return true; // Always scatters light
        }
    }
}
