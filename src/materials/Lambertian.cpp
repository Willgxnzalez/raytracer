#include "materials/Lambertian.h"
#include "core/HitRecord.h"
#include "math/Ray.h"
#include <random>

Vec3 randomInUnitSphere() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    while(true) {
        Vec3 v{dis(gen), dis(gen), dis(gen)};
        if (v.lengthSquared() < 1) return v;
    }
}

Lambertian::Lambertian(const Vec3& _albedo) : albedo(_albedo) {} 

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& record, Vec3& attenuation, Ray& scattered) const {
    Vec3 scatteredDirection = record.surfaceNormal + randomInUnitSphere();

    if (scatteredDirection.nearZero()) {
        scatteredDirection = record.surfaceNormal;
    }

    scattered = Ray{record.position, scatteredDirection};
    attenuation = albedo;
    return true;
}