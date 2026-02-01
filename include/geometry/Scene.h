#pragma once
#include "core/Hittable.h"
#include "BVHNode.h"

/**
 * Scene - Owns scene geometry and BVH acceleration structure
 */
struct Scene : Hittable {
    HittableList objects;
    std::unique_ptr<BVHNode> root;

    Scene() = default;

    void add(std::shared_ptr<Hittable> object);
    void clear();
    void build();

    virtual bool hit(
        HitRecord& record,
        const Ray& ray, 
        double tMin, 
        double tMax
    ) const override;

    AABB boundingBox() const override;
};