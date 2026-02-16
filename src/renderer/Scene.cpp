#include "Scene.h"

int Scene::addDiffuse(const Color& color) {
    Material m{};
    m.type = MaterialType::Diffuse;
    m.color = color;
    m.roughness = 0.0f;
    m.metallic = 0.0f;
    m.ior = 1.0f;
    m.emission = Color{0.0f, 0.0f, 0.0f};

    materials_.push_back(m);
    return static_cast<int>(materials_.size() - 1);
}

int Scene::addDielectric(float indexOfRefraction)
{
    Material m{};
    m.type = MaterialType::Dielectric;
    m.color = Vec3(1.0f, 1.0f, 1.0f);
    m.roughness = 0.0f;
    m.metallic = 0.0f;
    m.ior = indexOfRefraction;
    m.emission = Color{0.0f, 0.0f, 0.0f};

    materials_.push_back(m);
    return static_cast<int>(materials_.size() - 1);
}

int Scene::addSphere(const Point3& center, float radius, int materialIndex) {
    int index = static_cast<int>(spheres_.size());
    spheres_.push_back({
        center,
        radius,
        materialIndex
    });

    primitives_.push_back({
        PrimitiveType::Sphere,
        index
    });
    return index;
}

void Scene::build() {
    bvh_.build(*this);
}

bool Scene::intersect(
    HitRecord& record, 
    const Ray& ray, 
    float tMin, 
    float tMax
) const {
    return bvh_.hit(*this, record, ray, tMin, tMax);
}