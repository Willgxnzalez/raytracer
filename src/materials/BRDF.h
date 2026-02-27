#pragma once

#include "core/Vec3.h"

/*
 * BRDF (Bidirectional Reflectance Distribution Function)
 * Pure math describing how light reflects off a surface: f_r(wo, wi)
 *
 * A BRDF is reflection-only — no transmission, sampling, or RNG.
 * Examples: Lambertian, GGX, Fresnel-Schlick
 */

Vec3 fresnelSchlick(float cosTheta, const Color& F0);
float distributionGGX(float NdotH, float alpha);
float geometrySmith(float NdotV, float NdotL, float alpha);
