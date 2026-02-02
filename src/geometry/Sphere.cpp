#include "geometry/Sphere.h"
#include "geometry/AABB.h"
#include "core/HitRecord.h"
#include "core/Ray.h"
#include "materials/Material.h"
#include <atomic>

inline std::atomic<uint64_t> gSphereHits = 0;

Sphere::Sphere(const Vec3& center, double radius, const Material* material)
    : center(center), radius(radius), material(material) {}

bool Sphere::hit(
    HitRecord& record,
    const Ray& ray, 
    double tMin, 
    double tMax
) const {
    ++gSphereHits;
    // Points on sphere: |P - C|^2 = r^2 -> point P on sphere if its distance from center C is equal to radius
    // Points on ray:     P = O + tD 
    // Substitute: |O + tD - C|^2 = r^2
    // Rearrange: |(O - C) + tD|^2 = r^2 where (O - C) becomes vector oc
    // Expand: (D·D)t^2 + 2(D·oc)t + (oc·oc - r^2) = 0
    // Use "half-b" quadratic formula t = (-(b/2) +/- sqrt((b/2)^2-ac)) / a
    Vec3 oc = ray.origin - center;
    
    // Discriminant = (b/2)^2-ac
    double a = dot(ray.direction, ray.direction);
    double halfB = dot(ray.direction, oc);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = halfB * halfB - a * c;

    if (discriminant < 0) return false;

    double tMinus = (-halfB - std::sqrt(discriminant)) / a;
    double tPlus = (-halfB + std::sqrt(discriminant)) / a;

    double t;
    if (tMin <= tMinus && tMinus <= tMax ) 
        t = tMinus;
    else if (tMin <= tPlus && tPlus <= tMax ) 
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