#include "accel/BVH.h"
#include "core/HitRecord.h"
#include "geometry/Hittable.h"
#include <memory>
#include <stack>
#include <iostream>
#include <algorithm>

void BVHTree::build(const HittableList& objects)
{
    primitives_ = objects;

    size_t n = primitives_.size();
    if (n == 0) return;

    nodes_.clear();
    nodes_.reserve(n * 2);

    // Precompute primitive AABBs
    std::vector<BVHBuildEntry> entries;
    entries.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        entries.push_back({
            static_cast<int>(i),
            primitives_[i]->boundingBox()
        });
    }
    rootIndex_ = buildTree(entries, 0, n);
}

bool BVHTree::hit(
    HitRecord& record,
    const Ray& ray,
    float tMin,
    float tMax
) const {
    if (rootIndex_ < 0) return false;

    bool hitAnything = false;
    float closest = tMax;

    std::stack<int> stack;
    stack.push(rootIndex_);

    // Traverse tree until primitive hitbox is found
    while (!stack.empty()) {
        int nodeIndex = stack.top();
        stack.pop();

        const BVHNode& node = nodes_[nodeIndex];

        if (!node.box.hit(ray, tMin, closest)) continue;

        if (node.isLeaf() && primitives_[node.primitive]->hit(record, ray, tMin, closest)) {
            hitAnything = true;
            closest = record.t;
        } else {
            stack.push(node.right);
            stack.push(node.left);
        }
    }

    return hitAnything;
}

int BVHTree::buildTree(
    std::vector<BVHBuildEntry>& entries,
    size_t start,
    size_t end)
{
    int index = nodes_.size();
    nodes_.emplace_back(); // create node

    size_t n = end - start;
    
    if (n == 1) {
        nodes_[index].primitive = entries[start].primitiveIndex;
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
            float ca = (a.box.min[axis] + a.box.max[axis]) * 0.5f;
            float cb = (b.box.min[axis] + b.box.max[axis]) * 0.5f;
            return ca < cb;
        }
    );

    size_t mid = start + n / 2;

    nodes_[index].left = buildTree(entries, start, mid);
    nodes_[index].right = buildTree(entries, mid, end);
    nodes_[index].box = surroundingBox(
        nodes_[nodes_[index].left].box, 
        nodes_[nodes_[index].right].box
    );

    return index;
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