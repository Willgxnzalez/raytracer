#pragma once

#include "core/HitRecord.h"
#include "core/Vec3.h"
#include "core/Ray.h"
#include "accel/AABB.h"

/**
 * Sphere - A perfectly round 3D geometric primitive defined by a center point and radius.
 */
struct Sphere {
    Point3 center;
    float radius;
    int materialIndex;
};

bool sphereHit(
    const Sphere& sphere, 
    HitRecord& record,
    const Ray& ray, 
    float tMin, 
    float tMax
);

AABB sphereBounds(const Sphere& sphere);