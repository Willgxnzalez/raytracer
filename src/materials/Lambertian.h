#pragma once
#include "materials/Material.h"
#include "core/Vec3.h"

/**
 * Material that diffuses light creating a matte texture
 */
struct Lambertian : public Material {
    Color albedo; // Surface color

    Lambertian(const Vec3& albedo);

    bool scatter(
        const HitRecord& record, 
        const Ray& rayIn, 
        Ray& rayOut,
        Color& attenuation
    ) const override;
};
