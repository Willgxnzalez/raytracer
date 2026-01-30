#pragma once
#include "Hittable.h"
#include "../math/Vec3.h"

struct Material; // forward declare

/**
 * Sphere - A perfectly round 3D geometric primitive defined by a center point and radius.
 */
struct Sphere : public Hittable {
    Vec3 center;
    double radius;
    const Material* material;

    Sphere(const Vec3& center, double radius, const Material* material);

    bool hit(
        HitRecord& record,
        const Ray& ray, 
        double tMin, 
        double tMax
    ) const override;
};