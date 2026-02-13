#include "accel/BVHTree.h"
#include "geometry/Hittable.h"
#include <memory>
#include <algorithm>

void BVHTree::build(const HittableList& objects) {
    size_t n = objects.size();
    if(n < 1) return;

    // Precompute Leaf Node AABBs
    std::vector<BVHPrimitive> prims;
    prims.reserve(n);

    for (const auto& object: objects) {
        prims.push_back({
            object, 
            object->boundingBox()
        });
    }

    root_ = buildTree(prims, 0, n);
}

bool BVHTree::hit(
    HitRecord& record,
    const Ray& ray,
    float tMin,
    float tMax
) const {
    if (!root_) return false;
    return root_->hit(record, ray, tMin, tMax);
}

AABB BVHTree::boundingBox() const {
    if (!root_) return AABB();
    return root_->boundingBox();
}

std::unique_ptr<BVHNode> BVHTree::buildTree(
    std::vector<BVHPrimitive>& prims, 
    size_t start, 
    size_t end) 
{
    auto node = std::make_unique<BVHNode>();
    size_t n = end - start;

    if (n == 1) {
        auto prim = prims[start];
        node->leaf = prim.object;
        node->box = prim.box;
        return node;
    } else {
        // Choose axis with largest extent (SAH-lite)
        AABB bounds = prims[start].box;
        for (size_t i = start + 1; i < end; ++i) {
            bounds = surroundingBox(bounds, prims[i].box);
        }
        
        Vec3 extent = bounds.max - bounds.min;
        int axis = 0;
        if (extent.y > extent.x) axis = 1;
        if (extent.z > extent[axis]) axis = 2;

        // sort by box centroids
        std::sort(prims.begin() + start, prims.begin() + end, 
            [axis](const BVHPrimitive& a, const BVHPrimitive&b) {
                float centroidA = (a.box.min[axis] + a.box.max[axis]) * 0.5;
                float centroidB = (b.box.min[axis] + b.box.max[axis]) * 0.5;
                return centroidA < centroidB;
            }
        );
        size_t mid = start + n / 2;

        node->left = buildTree(prims, start, mid);
        node->right = buildTree(prims, mid, end);
        node->box = surroundingBox(node->left->box, node->right->box);

        return node;
    }
}

const BVHNode* BVHTree::root() { return root_.get(); }