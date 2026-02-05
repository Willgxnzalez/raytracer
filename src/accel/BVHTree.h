#pragma once
#include "geometry/Hittable.h"
#include "BVHNode.h"
#include <memory>
#include <vector>

class BVHTree {    
public:

    BVHTree() = default;

    void build(const HittableList & objects);

    bool hit(
        HitRecord& record,
        const Ray& ray,
        double tMin,
        double tMax
    ) const;

    AABB boundingBox() const;

    const BVHNode* root();
    
private:
    std::unique_ptr<BVHNode> root_;

    struct BVHPrimitive {
        std::shared_ptr<Hittable> object;
        AABB box;
    };

    std::unique_ptr<BVHNode> buildTree(std::vector<BVHPrimitive> & prims, size_t start, size_t end);
};