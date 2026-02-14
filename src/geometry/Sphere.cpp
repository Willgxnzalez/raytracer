#include "geometry/Sphere.h"
#include "accel/AABB.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include "materials/Material.h"
#include <atomic>

Sphere::Sphere(const Point3& center, float radius, const Material* material) : 
    center(center), 
    radius(radius), 
    material(material) 
{}

bool Sphere::hit(
    HitRecord& record,
    const Ray& ray, 
    float tMin, 
    float tMax
) const {
    // Points on sphere: |P - C|^2 = r^2 -> point P on sphere if its distance from center C is equal to radius
    // Points on ray:     P = O + tD 
    // Substitute: |O + tD - C|^2 = r^2
    // Rearrange: |(O - C) + tD|^2 = r^2 where (O - C) becomes vector oc
    // Expand: (D·D)t^2 + 2(D·oc)t + (oc·oc - r^2) = 0
    // Use "half-b" quadratic formula t = (-(b/2) +/- sqrt((b/2)^2-ac)) / a
    Vec3 oc = ray.origin - center;
    
    // Discriminant = (b/2)^2-ac
    float a = 1.0f; // dot(ray.direction, ray.direction)
    float halfB = dot(ray.direction, oc);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = halfB * halfB - a * c;

    if (discriminant < 0) return false;

    float tMinus = (-halfB - std::sqrt(discriminant)) / a;
    float tPlus = (-halfB + std::sqrt(discriminant)) / a;

    float t;
    if (tMin < tMinus && tMinus < tMax ) 
        t = tMinus;
    else if (tMin < tPlus && tPlus < tMax ) 
        t = tPlus;
    else return false;

    record.t = t;
    record.position = ray.at(t);
    Vec3 outwardNormal = (record.position - center) / radius;
    record.setFaceNormal(ray.direction, outwardNormal);
    record.material = material;
    return true;
}

AABB Sphere::boundingBox() const {
    return AABB{center - Vec3{radius, radius, radius}, center + Vec3{radius, radius, radius}};
}