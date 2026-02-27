#include "BRDF.h"

Vec3 fresnelSchlick(float cosTheta, const Color& F0) {
    float x = 1.0f - cosTheta;
    float x2 = x * x;
    float x5 = x2 * x2 * x;
    return F0 + (Vec3(1.0f) - F0) * x5;
}

float distributionGGX(float NdotH, float alpha){
    float a2 = alpha * alpha;
    float denom = (NdotH * NdotH) * (a2 - 1.0f) + 1.0f;
    return a2 / (std::numbers::pi_v<float> * denom * denom);
}


static float geometrySchlickGGX(float NdotV, float alpha){
    float k = (alpha + 1.0f);
    k = (k * k) / 8.0f;

    return NdotV / (NdotV * (1.0f - k) + k);
}

float geometrySmith(float NdotV, float NdotL, float alpha){
    return geometrySchlickGGX(NdotV, alpha) *
           geometrySchlickGGX(NdotL, alpha);
}
