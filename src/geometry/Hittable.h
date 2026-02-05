#pragma once
#include <vector>
#include <memory>

// forward declare
struct Ray;
struct HitRecord;
struct AABB;

/**
 * Hittable - interface for surfaces that can be hit by rays.
 */
struct Hittable {
    virtual ~Hittable() = default;

    /**
     * Test if a ray intersects this object within the given range.
     * 
     * @param record Populated with intersection data if hit occurs
     * @param ray Ray to test for intersection
     * @param tMin Minimum ray parameter to consider (near clip)
     * @param tMax Maximum ray parameter to consider (far clip)
     * @return true if ray hits object within [tMin, tMax], false otherwise
     */
    virtual bool hit(
        HitRecord& record,
        const Ray& ray, 
        double tMin, 
        double tMax
    ) const = 0;

    virtual AABB boundingBox() const = 0;
};

using HittableList = std::vector<std::shared_ptr<Hittable>>;
