#include "materials/BSDF.h"
#include "materials/Sampling.h"   // sampleCosineHemisphere, sampleGGX, ONB
#include "materials/BRDF.h"    // fresnelSchlick, distributionGGX, geometrySmith
#include <numbers>
#include <algorithm>

// Lobe mixing weight driven by average Fresnel reflectance at wo.
// Both Pdf and Sample must use this function so they stay in sync
static float specWeight(const Material& mat, const Vec3& N, const Vec3& wo) {
    float NdotV = std::max(dot(N, wo), 0.0f);
    Color F0 = lerp(Color(0.04f), mat.color, mat.metallic);
    Color F = fresnelSchlick(NdotV, F0);
    return std::clamp((F.x + F.y + F.z) / 3.0f, 0.05f, 0.95f);
}

// GGX specular PDF in solid-angle measure.
static float pdfGGX(float NdotH, float VdotH, float alpha) {
    float D = distributionGGX(NdotH, alpha);
    return (VdotH > 1e-6f) ? D * NdotH / (4.0f * VdotH) : 0.0f;
}

// Cosine-weighted diffuse PDF.
static float pdfDiffuse(float NdotL) {
    return std::max(NdotL, 0.0f) / std::numbers::pi_v<float>;
}

Color BSDF_Eval(
    const Material& material, 
    const HitRecord& record,
    const Vec3& wo, 
    const Vec3& wi)
{
    const Vec3& N = record.normal;

    switch (material.type) {
        case MaterialType::Diffuse:
        {
            if (dot(N, wi) <= 0.0f) return Color(0);
            return material.color / std::numbers::pi_v<float>;
        }

        case MaterialType::Metal:
        case MaterialType::Physical:
        {
            float NdotV = std::max(dot(N, wo), 0.0f);
            float NdotL = std::max(dot(N, wi), 0.0f);
            if (NdotV <= 0.0f || NdotL <= 0.0f) return Color(0);

            Vec3  H     = (wo + wi).normalized();
            float NdotH = std::max(dot(N, H), 0.0f);
            float VdotH = std::max(dot(wo, H), 0.0f);
            float alpha = std::max(material.roughness * material.roughness, 0.001f);

            Color F0 = lerp(Color(0.04f), material.color, material.metallic);
            Color F  = fresnelSchlick(VdotH, F0);
            float D  = distributionGGX(NdotH, alpha);
            float G  = geometrySmith(NdotV, NdotL, alpha);

            Color specular = F * D * G / std::max(4.0f * NdotV * NdotL, 1e-6f);
            Color diffuse  = material.color * (1.0f - material.metallic) / std::numbers::pi_v<float>;

            return specular + diffuse;
        }

        case MaterialType::Dielectric:
            return Color(0);
        default:
            return Color(0);
    }
}

float BSDF_Pdf(
    const Material& material, 
    const HitRecord& record,
    const Vec3& wo, 
    const Vec3& wi)
{
    const Vec3& N = record.normal;

    switch (material.type) {
        case MaterialType::Diffuse:
            return pdfDiffuse(dot(N, wi));

        case MaterialType::Metal:
        case MaterialType::Physical:
        {
            float NdotV = std::max(dot(N, wo), 0.0f);
            float NdotL = std::max(dot(N, wi), 0.0f);
            if (NdotV <= 0.0f || NdotL <= 0.0f) return 0.0f;

            Vec3  H     = (wo + wi).normalized();
            float NdotH = std::max(dot(N, H), 0.0f);
            float VdotH = std::max(dot(wo, H), 0.0f);
            float alpha = std::max(material.roughness * material.roughness, 0.001f);

            float pSpec = specWeight(material, N, wo);

            return pSpec * pdfGGX(NdotH, VdotH, alpha) + (1.0f - pSpec) * pdfDiffuse(NdotL);
        }
        case MaterialType::Dielectric:
            return 0.0f; // Delta distribution — pdf is conceptually infinite; integrator handles via f/pdf=1
        default:
            return 0.0f;
    }
}

BSDFSample BSDF_Sample(
    const Material& material,  
    const HitRecord& record,   
    const Vec3& wo, 
    RNG& rng)
{
    BSDFSample s{};
    const Vec3& N = record.normal;

    switch (material.type) {
        case MaterialType::Diffuse: 
        {
            ONB onb{N};
            s.wi  = onb.toWorld(sampleCosineHemisphere(rng));
            s.f   = BSDF_Eval(material, record, wo, s.wi);
            s.pdf = BSDF_Pdf (material, record, wo, s.wi);
            return s;
        }

        case MaterialType::Metal:
        case MaterialType::Physical:
        {
            float pSpec = specWeight(material, N, wo);
            float alpha = std::max(material.roughness * material.roughness, 0.001f);

            if (rng.uniform01() < pSpec) { // Specular lobe
                Vec3 H = sampleGGX(N, alpha, rng);
                Vec3 L = reflect(-wo, H);

                if (dot(N, L) <= 0.0f) { 
                    s.pdf = 0.0f; 
                    return s; 
                }
                s.wi = L;
            } else { // Diffuse lobe
                ONB onb{N};
                s.wi = onb.toWorld(sampleCosineHemisphere(rng));
            }

            s.f   = BSDF_Eval(material, record, wo, s.wi);
            s.pdf = BSDF_Pdf (material, record, wo, s.wi);
            return s;
        }

        case MaterialType::Dielectric:
        {
            float eta      = material.ior;
            float cosTheta = std::min(dot(wo, N), 1.0f);
            float R0       = (1.0f - eta) / (1.0f + eta);
            Color F0       = Color(R0 * R0);
            float reflProb = fresnelSchlick(cosTheta, F0).x;
            float etaRatio = record.frontFace ? (1.0f / eta) : eta;
            float sinTheta2 = 1.0f - cosTheta * cosTheta;
            bool totalInternalReflection = (etaRatio * etaRatio * sinTheta2) > 1.0f;

            if (totalInternalReflection || rng.uniform01() < reflProb)
                s.wi = reflect(-wo, N);
            else
                s.wi = refract(-wo, N, etaRatio);

            s.f   = Color(1.0f);
            s.pdf = 1.0f;
            return s;
        }

        default:
            s.pdf = 0.0f;
            return s;
    }
}