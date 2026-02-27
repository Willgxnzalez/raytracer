#pragma once
#include "accel/BVH.h"
#include "geometry/Sphere.h"
#include "materials/Material.h"
#include "core/Vec3.h"
#include <vector>
#include <cstdint>

/**
 * Scene - Owns scene geometry and BVH acceleration structure
 */
class Scene {
public:
    enum class PrimitiveType : uint8_t {
        Sphere,
        Triangle,
        Plane
    };
    
    struct PrimitiveRef {
        PrimitiveType type;
        int index;
    };
    
    // Material creation
    int addDiffuse(const Color& color);
    int addMetal(const Color& color, float roughness = 0.0f);
    int addPhysical(const Color& color, float metallic = 0.0f, float roughness = 0.0f);
    int addDielectric(float ior);
    int addEmissive(const Color& color);
    
    // Geometry creation
    int addSphere(const Point3& center, float radius, int materialIndex);
    
    // Build acceleration structure
    void build();
    
    bool intersect(
        HitRecord& record, 
        const Ray& ray, 
        float tMin, 
        float tMax
    ) const;
    
    // Read-only access
    const std::vector<Sphere>& getSpheres() const { return spheres_; }
    const std::vector<Material>& getMaterials() const { return materials_; }
    const std::vector<PrimitiveRef>& getPrimitives() const { return primitives_; }
    const BVHTree& getBVH() const { return bvh_; }
    
private:
    std::vector<Sphere> spheres_;
    std::vector<Material> materials_;
    std::vector<PrimitiveRef> primitives_;
    BVHTree bvh_;
};