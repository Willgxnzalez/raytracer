#pragma once
#include "core/Vec3.h"
#include "core/Ray.h"
#include <iostream>
#include <atomic>

/**
 * Axis-Aligned Bounding Box
 * Defined by minimum and maximum corners.
 */
struct AABB {
    Vec3 min;
    Vec3 max;

    AABB() = default;
    AABB(const Vec3& minCorner, const Vec3& maxCorner) : min(minCorner), max(maxCorner) {}

    inline bool hit(const Ray& ray, float tMin, float tMax) const {
        constexpr float EPS = 1e-5f;

        for (int axis = 0; axis < 3; ++axis) {
            float dir = ray.direction[axis];
            float t0, t1;

            if (std::abs(dir) > EPS) { // Treat nearly parallel rays as parellel to avoid numerical blowup 
                float invDir = 1.0 / dir;
                t0 = (min[axis] - ray.origin[axis]) * invDir;
                t1 = (max[axis] - ray.origin[axis]) * invDir;
                if (invDir < 0) std::swap(t0, t1);
            } else { // Ray parallel to slab
                
                if (ray.origin[axis] < min[axis] || ray.origin[axis] > max[axis])
                    return false;
                t0 = -INFINITY;
                t1 = INFINITY;
            }

            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;

            if (tMax < tMin)
                return false;
        }
        return true;
    }

};

static AABB surroundingBox(const AABB& box0, const AABB& box1) {
    return AABB{
        Vec3{std::min(box0.min.x, box1.min.x),
             std::min(box0.min.y, box1.min.y),
             std::min(box0.min.z, box1.min.z)},
        Vec3{std::max(box0.max.x, box1.max.x),
             std::max(box0.max.y, box1.max.y),
             std::max(box0.max.z, box1.max.z)}
    };
}

inline std::ostream & operator << ( std::ostream & out, const AABB & box) {
    out << "[" << box.min << ", " << box.max << "]";
    return out;
}