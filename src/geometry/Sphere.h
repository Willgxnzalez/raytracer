#pragma once
#include "geometry/Hittable.h"
#include "core/Vec3.h"

struct Material; // forward declare

/**
 * Sphere - A perfectly round 3D geometric primitive defined by a center point and radius.
 */
struct Sphere : public Hittable {
    Vec3 center;
    float radius;
    const Material* material;

    Sphere(const Vec3& center, float radius, const Material* material);

    bool hit(
        HitRecord& record,
        const Ray& ray, 
        float tMin, 
        float tMax
    ) const override;

    AABB boundingBox() const override;
};

