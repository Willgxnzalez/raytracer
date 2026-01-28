#pragma once
#include "Hittable.h"
#include "../math/Vec3.h"

struct Material; // forward declare

/**
 * Sphere - A perfectly round 3D geometric primitive defined by a center point and radius.
 * 
 * Implements ray-sphere intersection using the quadratic formula by solving:
 *   ||ray.at(t) - center||^2 = radius^2.
 */
struct Sphere : public Hittable {
    Vec3 center;
    double radius;
    const Material* material;

    Sphere(const Vec3& _center, double _radius, const Material* _material);

    bool hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const override;
};