#pragma once
#include "Hittable.h"
#include "../math/Vec3.h"
#include "../materials/Material.h"
#include <cmath>

struct Sphere : public Hittable {
    Vec3 center;
    double radius;
    const Material* material;

    Sphere(const Vec3& _center, double _radius): center(_center), radius(_radius) {}

    bool hit(const Ray& ray, double tMin, double tMax, HitRecord& record) const override {
        // Vector from ray origin to center
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
        record.setFaceNormal(ray, outwardNormal);
        return true;
    }
};