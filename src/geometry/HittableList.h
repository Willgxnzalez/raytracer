#pragma once
#include "Hittable.h"
#include <vector>
#include <memory>

struct HittableList : public Hittable {
    std::vector<std::shared_ptr<Hittable>> objects;
    
    HittableList() = default;

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    void clear() {
        objects.clear();
    }


    bool hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const override {
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

};