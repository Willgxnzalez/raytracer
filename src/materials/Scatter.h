#pragma once

#include "materials/Material.h"
#include "core/Vec3.h"
#include "core/Ray.h"
#include "core/HitRecord.h"
#include "util/RNG.h"


/**
 * Compute how light scatters when hitting this material.
 * 
 * @param material Material to scatter light off of
 * @param record Surface intersection details (position, normal, etc.)
 * @param rayIn Incoming ray that hit the surface
 * @param rayOut Output: scattered/reflected ray direction
 * @param attenuation Output: color absorption (how much light is kept vs absorbed)
 * @param rng Random number generator
 * @return true if ray scatters, false if ray is absorbed (stops bouncing)
 */
bool scatter(
    const Material& material,
    const HitRecord& rec,
    const Ray& rayIn,
    Ray& rayOut,
    Vec3& attenuation,
    RNG& rng
);
