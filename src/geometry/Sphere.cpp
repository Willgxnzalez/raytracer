#include "geometry/Sphere.h"
#include "core/HitRecord.h"
#include "materials/Material.h"
#include "math/Ray.h"

Sphere::Sphere(const Vec3& _center, double _radius, const Material* _material): center(_center), radius(_radius), material(_material) {}

bool Sphere::hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const {
    // Points on sphere: |P - C|^2 = r^2 -> point P on sphere if its distance from center C is equal to radius
    // Points on ray:     P = O + tD 
    // Substitute: |O + tD - C|^2 = r^2
    // Rearrange: |(O - C) + tD|^2 = r^2 where (O - C) becomes vector oc
    // Expand: (D·D)t^2 + 2(D·oc)t + (oc·oc - r^2) = 0
    // Use "half-b" quadratic formula t = (-(b/2) +/- sqrt((b/2)^2-ac)) / a
    Vec3 oc = ray.origin - center;
    
    // Discriminant
    double a = dot(ray.direction, ray.direction);
    double halfB = dot(ray.direction, oc);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = halfB * halfB - a * c;

    if (discriminant < 0) return false;

    // t = (-halfB +/- sqrt(discriminant)) / a
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