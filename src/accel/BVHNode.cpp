#include "accel/BVHNode.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include <algorithm>
#include <iostream>

bool BVHNode::hit(
    HitRecord& record, 
    const Ray& ray, 
    double tMin, 
    double tMax
) const {
    if (!box.hit(ray, tMin, tMax)) {
        return false;
    }

    if (leaf) {
        return leaf->hit(record, ray, tMin, tMax);
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

bool BVHNode::isLeaf() const {
    return leaf != nullptr;
}