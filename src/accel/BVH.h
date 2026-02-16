#pragma once

#include "accel/AABB.h"
#include "core/HitRecord.h"
#include "core/Vec3.h"
#include "core/Ray.h"
#include <vector>

class Scene;  // Forward declare

static constexpr int InvalidNode = -1;

/**
 * BVH Node - contains two children or references a primitive (leaf)
 */
struct BVHNode {
    int left;            // Index of left child (-1 if leaf)
    int right;           // Index of right child (-1 if leaf)
    int primitiveIndex;  // Index into primitives array

    AABB box;

    inline bool isLeaf() const { return primitiveIndex > InvalidNode; }
};

/**
 * Bounding Volume Hierarchy for ray-scene intersection acceleration
 */
class BVHTree {
public:
    BVHTree() = default;

    void build(const Scene& scene);

    bool hit(
        const Scene& scene,
        HitRecord& record,
        const Ray& ray,
        float tMin,
        float tMax
    ) const;

    AABB boundingBox() const;

    const BVHNode& root();
    
private:
    std::vector<BVHNode> nodes_;
    int rootIndex_ = InvalidNode;

    struct BVHBuildEntry {
        int primitiveIndex;
        AABB box;
        Point3 centroid;
    };

    int buildTree(std::vector<BVHBuildEntry>& entries, size_t start, size_t end);
};