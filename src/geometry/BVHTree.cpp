#include "geometry/BVHTree.h"
#include "core/Hittable.h"
#include <memory>
#include <random>
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
    double tMin,
    double tMax
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
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> axisDis(0, 2);
        int axis = axisDis(gen);

        // sort by box centroids
        std::sort(prims.begin() + start, prims.begin() + end, 
            [axis](const BVHPrimitive& a, const BVHPrimitive&b) {
                double centroidA = (a.box.min[axis] + a.box.max[axis]) * 0.5;
                double centroidB = (b.box.min[axis] + b.box.max[axis]) * 0.5;
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