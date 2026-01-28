#pragma once
#include "../math/Ray.h"
#include "../math/Vec3.h"
#include "../core/HitRecord.h"


/**
 * Material - interface for describing light interaction on a surface.
 */

struct Material {
    virtual ~Material() = default;

    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Vec3& attenuation, Ray& scattered) const = 0;
};
