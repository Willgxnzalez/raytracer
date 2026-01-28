#pragma once
#include "Hittable.h"
#include <vector>
#include <memory>

struct HittableList : public Hittable {
    std::vector<std::shared_ptr<Hittable>> objects;
    
    HittableList() = default;

    void add(std::shared_ptr<Hittable> object);
    void clear();
    
    bool hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const override;
};