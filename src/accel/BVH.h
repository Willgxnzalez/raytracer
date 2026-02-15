#pragma once
#include "accel/AABB.h"
#include "geometry/Hittable.h"
#include <vector>

/**
 * BVH Node - internal representation
 * Each node either contains two children or references a primitive (leaf)
 */
struct BVHNode {
    int left = -1;       // Index of left child (-1 if leaf)
    int right = -1;      // Index of right child (-1 if leaf)
    int primitive = -1;  // Index into primitives array (-1 if internal node)
    
    AABB box;

    bool isLeaf() const { return primitive >= 0; }
};

/**
 * Bounding Volume Hierarchy for ray-scene intersection acceleration
 */
class BVHTree {    
    std::vector<BVHNode> nodes_;
    HittableList primitives_;
    int rootIndex_ = -1;
    
public:

    BVHTree() = default;

    void build(const HittableList & objects);

    bool hit(
        HitRecord& record,
        const Ray& ray,
        float tMin,
        float tMax
    ) const;

    AABB boundingBox() const;

    const BVHNode& root();
    
private:
    struct BVHBuildEntry {
        int primitiveIndex;
        AABB box;
    };

    int buildTree(std::vector<BVHBuildEntry>& prims, size_t start, size_t end);
};