#pragma once
#include "accel/BVH.h"

/**
 * Scene - Owns scene geometry and BVH acceleration structure
 */
class Scene {
    HittableList objects;
    BVHTree bvh;

public:
    Scene() = default;

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    void build() {
        bvh.build(objects);
    }

    bool intersect(
        HitRecord& record,
        const Ray& ray, 
        float tMin, 
        float tMax
    ) const {
        return bvh.hit(record, ray, tMin, tMax);
    }
};
