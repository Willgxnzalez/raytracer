#pragma once
#include "geometry/Hittable.h"
#include "accel/AABB.h"

/**
 * Bounding Volume Hierarchy Node
 * Each node either:
 * - contains two child nodes
 * - contains a single leaf node
 */
struct BVHNode : public Hittable {
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::shared_ptr<Hittable> leaf;
    AABB box;

    BVHNode() = default;

    bool hit(
        HitRecord& record,
        const Ray& ray,
        float tMin,
        float tMax
    ) const override;

    AABB boundingBox() const override;

    bool isLeaf() const;
};