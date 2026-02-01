#include "geometry/Scene.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include <memory>
#include <iostream>


void Scene::add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
}

void Scene::clear() {
    objects.clear();
}

void Scene::build() {
    root = std::make_unique<BVHNode>(objects, 0, objects.size());
}

bool Scene::hit(
        HitRecord& record,
        const Ray& ray, 
        double tMin, 
        double tMax
) const {
    return root->hit(record, ray, tMin, tMax);
}

AABB Scene::boundingBox() const {
    if (!root) return AABB();
    return root->boundingBox();
}


// bool World::hit(
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