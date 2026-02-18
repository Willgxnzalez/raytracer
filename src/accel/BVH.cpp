#include "accel/BVH.h"
#include "renderer/Scene.h"
#include <memory>
#include <stack>
#include <iostream>
#include <algorithm>

void BVHTree::build(const Scene& scene) {
    const auto& primitives = scene.getPrimitives();

    size_t n = primitives.size();
    if (n == 0) return;

    nodes_.clear();
    nodes_.reserve(n * 2);

    // Precompute primitive AABBs
    std::vector<BVHBuildEntry> entries;
    entries.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        const auto& prim = primitives[i];

        AABB box;
        switch (prim.type) {
            case Scene::PrimitiveType::Sphere:
                box = sphereBounds(scene.getSpheres()[prim.index]);
        }
        Point3 centroid = (box.min + box.max) * 0.5f;
        
        entries.push_back({static_cast<int>(i), box, centroid});
    }
    
    rootIndex_ = buildTree(entries, 0, n);
}

int BVHTree::buildTree(
    std::vector<BVHBuildEntry>& entries,
    size_t start,
    size_t end)
{
    int index = static_cast<int>(nodes_.size());
    nodes_.emplace_back();

    size_t n = end - start;
    
    if (n == 1) {
        nodes_[index].left = InvalidNode;
        nodes_[index].right = InvalidNode;
        nodes_[index].primitiveIndex = entries[start].primitiveIndex;
        nodes_[index].box = entries[start].box;
        return index;
    }

    // Compute bounds
    AABB bounds = entries[start].box;
    for (size_t i = start + 1; i < end; ++i)
        bounds = surroundingBox(bounds, entries[i].box);

    Vec3 extent = bounds.max - bounds.min;
    int axis = 0;
    if (extent.y > extent.x) axis = 1;
    if (extent.z > extent[axis]) axis = 2;

    std::sort(
        entries.begin() + start,
        entries.begin() + end,
        [axis](const BVHBuildEntry& a, const BVHBuildEntry& b) {
            return a.centroid[axis] < b.centroid[axis];
        }
    );

    size_t mid = start + n / 2;

    nodes_[index].left = buildTree(entries, start, mid);
    nodes_[index].right = buildTree(entries, mid, end);
    nodes_[index].primitiveIndex = InvalidNode;
    nodes_[index].box = surroundingBox(
        nodes_[nodes_[index].left].box, 
        nodes_[nodes_[index].right].box
    );

    return index;
}

bool BVHTree::hit(
    const Scene& scene,
    HitRecord& record,
    const Ray& ray,
    float tMin,
    float tMax
) const {
    if (rootIndex_ < 0) return false;

    bool hitAnything = false;
    float closest = tMax;

    int stack[64]; // Tree with 64 levels could hold 2^64 leaf nodes = 1.8x10^19 objects
    int stackPtr = 0; // Next available spot
    stack[stackPtr++] = rootIndex_;

    // Traverse tree until primitive hitbox is found
    while (stackPtr > 0) {
        int nodeIndex = stack[--stackPtr];

        const BVHNode& node = nodes_[nodeIndex];
        const auto& prim = scene.getPrimitives()[node.primitiveIndex];

        if (!node.box.hit(ray, tMin, closest)) continue;

        if (node.isLeaf()) {
            switch (prim.type) {
                case Scene::PrimitiveType::Sphere: 
                    if (sphereHit(scene.getSpheres()[prim.index], record, ray, tMin, closest)) {
                        hitAnything = true;
                        closest = record.t;
                    }
            }
        } else {
            stack[stackPtr++] = node.right;
            stack[stackPtr++] = node.left;
        }
    }

    return hitAnything;
}


AABB BVHTree::boundingBox() const {
    if (rootIndex_ < 0 || nodes_.empty()) {
        return AABB();
    }
    return nodes_[rootIndex_].box;
}

const BVHNode& BVHTree::root() { 
    return nodes_[rootIndex_]; 
}