#include "geometry/HittableList.h"
#include "core/HitRecord.h"
#include "math/Ray.h"

void HittableList::add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
}

void HittableList::clear() {
    objects.clear();
}

bool HittableList::hit(
    HitRecord& record,
    const Ray& ray, 
    double tMin, 
    double tMax
) const {
    HitRecord tempRecord;
    double tClosest = tMax;
    bool hitDetected = false;

    // Find closest intersection by progressively narrowing search range
    for (const auto& object : objects) {
        if (object->hit(tempRecord, ray, tMin, tClosest)) { // Use closest hit so far as new tMax
            hitDetected = true;
            tClosest = tempRecord.t; // Update closest distance
            record = tempRecord; // save this hit
        }
    }

    return hitDetected;
}