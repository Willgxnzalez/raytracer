#include "materials./Sampling.h"

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

Vec3 sampleCosineHemisphere(RNG& rng) {
    float r1 = rng.uniform01();
    float r2 = rng.uniform01();

    float phi = 2.0f * std::numbers::pi_v<float> * r1;
    float x = std::cos(phi) * std::sqrt(r2);
    float y = std::sin(phi) * std::sqrt(r2);
    float z = std::sqrt(1.0f - r2);

    return Vec3(x, y, z);
}

Vec3 reflect(const Vec3& dir, const Vec3& normal) {
    return dir - 2.0f * dot(dir, normal) * normal;
}

Vec3 refract(const Vec3& dir, const Vec3& normal, float iorRatio) {
    float cosTheta = std::min(dot(-dir, normal), 1.0f);
    Vec3 perp = iorRatio * (dir + cosTheta * normal); // Perpendicular component of refracted ray
    Vec3 parallel = -std::sqrt(1.0f - perp.lengthSquared()) * normal; // Parallel component of refracted ray
    return perp + parallel;
}

Vec3 sampleGGX(const Vec3& N, float alpha, RNG& rng) {
    float r1 = rng.uniform01();
    float r2 = rng.uniform01();

    float phi = 2.0f * std::numbers::pi_v<float> * r1;

    float cosTheta = std::sqrt((1.0f - r2) / (1.0f + (alpha*alpha - 1.0f) * r2));
    float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

    Vec3 H_local(
        sinTheta * std::cos(phi),
        sinTheta * std::sin(phi),
        cosTheta
    );

    ONB onb{N};
    return onb.toWorld(H_local);
}


