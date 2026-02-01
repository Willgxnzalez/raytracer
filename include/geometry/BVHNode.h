#pragma once
#include "core/Hittable.h"
#include "AABB.h"

/**
 * Bounding Volume Hierarchy Node
 * Each node either:
 * - contains two child nodes, or
 * - contains a single leaf node
 */
struct BVHNode : public Hittable {
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;

    BVHNode(HittableList& objects, size_t start, size_t end);

    bool hit(
        HitRecord& record,
        const Ray& ray,
        double tMin,
        double tMax
    ) const override;

    AABB boundingBox() const override;
};