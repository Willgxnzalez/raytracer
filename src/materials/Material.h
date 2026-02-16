#pragma once

#include "core/Ray.h"
#include "core/Vec3.h"
#include "util/RNG.h"


enum class MaterialType : uint8_t {
    Diffuse,    // Pure Lambertian
    Metal,      // Perfect Mirror or rough metal
    Physical,   // Diffuse + Specular
    Dielectric, // Transparent material that refracts and refrects light
    Emissive
};

/**
 * Material - Describes light interaction on a surface.
 */
struct Material {
    MaterialType type;
    Color color;

    float roughness; // 0.0 = perfect mirror, 1.0 = blurry reflection / brushed surface
    float metallic;  // 0.0 = pure diffuse, 1.0 = pure metal
    float ior;       // Index of refraction: how much light bends (air=1.0, water=1.33, glass=1.5, diamond=2.4)

    Color emission;
};