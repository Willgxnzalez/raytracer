#include "geometry/HittableList.h"
#include "core/HitRecord.h"
#include "math/Ray.h"

void HittableList::add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
}

void HittableList::clear() {
    objects.clear();
}

bool HittableList::hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const {
    HitRecord tempRecord;
    double tClosest = tMax;
    bool hitDetected = false;

    for (const auto& object : objects) {
        if (object->hit(ray, tMin, tClosest, tempRecord)) {
            hitDetected = true;
            tClosest = tempRecord.t;
            record = tempRecord;
        }
    }

    return hitDetected;
}