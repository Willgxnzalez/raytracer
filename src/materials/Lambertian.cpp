#include "materials/Lambertian.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include <random>

Vec3 randomInUnitSphere() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dis(-1.0, 1.0);
    while(true) {
        Vec3 v{dis(gen), dis(gen), dis(gen)};
        if (v.lengthSquared() < 1) return v;
    }
}

Lambertian::Lambertian(const Vec3& _albedo) : albedo(_albedo) {} 

bool Lambertian::scatter(
    const HitRecord& record, 
    const Ray& rayIn, 
    Ray& rayOut,
    Vec3& attenuation 
) const {
    Vec3 scatteredDirection = record.normal + randomInUnitSphere();

    if (scatteredDirection.nearZero()) {
        scatteredDirection = record.normal;
    }

    rayOut = Ray{record.position, scatteredDirection};
    attenuation = albedo;
    return true; // Always scatters light
}