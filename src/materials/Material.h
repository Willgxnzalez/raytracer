#pragma once
#include "core/Ray.h"
#include "core/Vec3.h"

struct HitRecord; // forward declare

/**
 * Material - interface for describing light interaction on a surface.
 */
struct Material {
    virtual ~Material() = default;

    /**
     * Compute how light scatters when hitting this material.
     * 
     * @param record Surface intersection details (position, normal, etc.)
     * @param rayIn Incoming ray that hit the surface
     * @param rayOut Output: scattered/reflected ray direction
     * @param attenuation Output: color absorption (how much light is kept vs absorbed)
     * @return true if ray scatters, false if ray is absorbed (stops bouncing)
     */
    virtual bool scatter(
        const HitRecord& record, 
        const Ray& rayIn, 
        Ray& rayOut,
        Vec3& attenuation 
    ) const = 0;
};
