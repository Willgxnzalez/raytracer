#pragma once
#include "Material.h"

/**
 * Material that diffuses light creating a matte texture
 */
struct Lambertian : public Material {
    Vec3 albedo;

    Lambertian(const Vec3& _albedo);

    bool scatter(const Ray& rayIn, const HitRecord& record, Vec3& attenuation, Ray& scattered) const override;
};
