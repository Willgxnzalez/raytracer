#pragma once
#include "materials/Material.h"

/**
 * Transparent material that refracts and refrects light
 */
struct Dielectric : public Material {
    float eta; // Index of refraction: how much light bends (air=1.0, water=1.33, glass=1.5, diamond=2.4)

    Dielectric(float eta);

    bool scatter(
        const HitRecord& record, 
        const Ray& rayIn, 
        Ray& rayOut,
        Vec3& attenuation 
    ) const override;
};