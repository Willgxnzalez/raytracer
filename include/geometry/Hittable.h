#pragma once

// forward declare
struct Ray;
struct HitRecord;

/**
 * Hittable - interface for surfaces that can be hit by rays.
 */
struct Hittable {
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const = 0;
};
