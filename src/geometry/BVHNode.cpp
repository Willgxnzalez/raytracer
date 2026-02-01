#include "geometry/BVHNode.h"
#include "geometry/AABB.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include <random>
#include <algorithm>
#include <iostream>

BVHNode::BVHNode(HittableList& objects, size_t start, size_t end) {
    size_t n = end - start;

    if (n == 1) {
        left = right = objects[start];
    } else if (n == 2) {
        left = objects[start];
        right = objects[start + 1];
    } else {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> axisDis(0, 2);
        int axis = axisDis(gen);

        auto comparator = [axis](const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
            return a->boundingBox().min[axis] < b->boundingBox().min[axis];
        };

        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        size_t mid = start + n / 2;

        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }

    // Compute bounding box
    AABB boxLeft = left->boundingBox();
    AABB boxRight = right->boundingBox();
    box = surroundingBox(boxLeft, boxRight);
}

bool BVHNode::hit(
    HitRecord& record, 
    const Ray& ray, 
    double tMin, 
    double tMax
) const {
    if (!box.hit(ray, tMin, tMax)){
        //std::cout << "Failing here Min Max" << tMin << " " << tMax << std::endl;
        return false;
    }

    HitRecord leftRec, rightRec;
    bool hitLeft = left->hit(leftRec, ray, tMin, tMax);
    bool hitRight = right->hit(rightRec, ray, tMin, hitLeft ? leftRec.t : tMax);

    if (hitLeft && hitRight) {
        record = (leftRec.t < rightRec.t) ? leftRec : rightRec;
        return true;
    } else if (hitLeft) {
        record = leftRec;
        return true;
    } else if (hitRight) {
        record = rightRec;
        return true;
    }
    return false;
}

AABB BVHNode::boundingBox() const {
    return box;
}