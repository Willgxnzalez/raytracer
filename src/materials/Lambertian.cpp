#include "materials/Lambertian.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include <random>

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

Lambertian::Lambertian(const Color& _albedo) : albedo(_albedo) {} 

bool Lambertian::scatter(
    const HitRecord& record, 
    const Ray& rayIn, 
    Ray& rayOut,
    Color& attenuation,
    RNG& rng 
) const {
    Vec3 scatteredDirection = record.normal + randomInUnitSphere(rng);

    if (scatteredDirection.nearZero()) {
        scatteredDirection = record.normal;
    }

    rayOut = Ray{record.position, scatteredDirection};
    attenuation = albedo;
    return true; // Always scatters light
}