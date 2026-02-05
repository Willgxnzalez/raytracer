#pragma once
#include "geometry/Hittable.h"
#include "accel/BVHTree.h"

/**
 * Scene - Owns scene geometry and BVH acceleration structure
 */
struct Scene : Hittable {
    HittableList objects;
    BVHTree bvh;

    Scene() = default;

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    void build() {
        bvh.build(objects);
    }

    virtual bool hit(
        HitRecord& record,
        const Ray& ray, 
        double tMin, 
        double tMax
    ) const override {
        return bvh.hit(record, ray, tMin, tMax);
    }

    AABB boundingBox() const override {
        return bvh.boundingBox();
    }
};

// bool hit(
//     HitRecord& record,
//     const Ray& ray, 
//     double tMin, 
//     double tMax
// ) const {
//     HitRecord tempRecord;
//     double tClosest = tMax;
//     bool hitDetected = false;

//     // Find closest intersection by progressively narrowing search range
//     for (const auto& object : objects) {
//         if (object->hit(tempRecord, ray, tMin, tClosest)) { // Use closest hit so far as new tMax
//             hitDetected = true;
//             tClosest = tempRecord.t; // Update closest distance
//             record = tempRecord; // save this hit
//         }
//     }

//     return hitDetected;
// }