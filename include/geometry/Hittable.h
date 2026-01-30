#pragma once

// forward declare
struct Ray;
struct HitRecord;

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
};
