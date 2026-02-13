#include "materials/Dielectric.h"
#include "core/Ray.h"
#include "core/Vec3.h"
#include "core/HitRecord.h"
#include <cmath>

Dielectric::Dielectric(float eta) : eta(eta) {}

bool Dielectric::scatter(
    const HitRecord& record,
    const Ray& rayIn,
    Ray& rayOut,
    Color& attenuation,
    RNG& rng
) const {

    // Dielectrics do not absorb light
    attenuation = Color(1.0f, 1.0f, 1.0f);

    // Normalize incoming direction
    Vec3 unitDir = rayIn.direction.normalized();

    // Determine refraction ratio depending on ray direction
    // If hitting front face: air -> material
    // If inside material: material -> air
    float etaRatio = record.frontFace ? (1.0f / eta) : eta;

    // Cosine of angle between ray and surface normal
    float cosTheta = std::min(dot(-unitDir, record.normal), 1.0f);

    // Compute perpendicular component of refracted ray
    Vec3 r_perp = etaRatio * (unitDir + cosTheta * record.normal);

    // Check for total internal reflection
    bool cannotRefract = r_perp.lengthSquared() > 1.0f;

    // Schlick approximation for reflectance
    float R0 = (1.0f - eta) / (1.0f + eta);
    R0 = R0 * R0;
    float R = R0 + (1.0f - R0) * std::pow(1.0f - cosTheta, 5.0f);

    if (cannotRefract || rng.uniform01() < R) {
        // Reflect
        Vec3 reflected = unitDir - 2.0f * dot(unitDir, record.normal) * record.normal;
        rayOut = Ray(record.position, reflected.normalized());
    } else {
        // Refract
        Vec3 r_parallel = -std::sqrt(1.0f - r_perp.lengthSquared()) * record.normal;
        Vec3 refracted = r_perp + r_parallel;
        rayOut = Ray(record.position, refracted.normalized());
    }
    return true; // Always scatters light
}
