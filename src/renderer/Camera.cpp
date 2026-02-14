#include "Camera.h"
#include <random>
#include <numbers>
#include <cmath>
#include <iostream>

float degreesToRadians(float degrees) {
    return degrees * (std::numbers::pi / 180);
}

Point3 randomPointOnUnitDisk(RNG& rng) { // in x-y plane
    float angle = rng.uniform(0.0f, 2.0f * std::numbers::pi);
    float radius = rng.uniform01();
    return Point3{std::cos(angle) * radius, std::sin(angle) * radius, 0.0f};
}

Camera::Camera(
    const Point3& lookFrom, 
    const Point3& lookAt, 
    const Vec3& vUp, 
    int imageWidth, 
    int imageHeight, 
    float vFovDegrees,
    float aperture,
    float focusDistance
) :
    origin_(lookFrom), 
    imageWidth_(imageWidth), 
    imageHeight_(imageHeight), 
    aperture_(aperture), 
    focusDistance_(focusDistance)
{
    // Using Tan(vFov/2) = (viewportHeight/2) / 1 (focalLength)
    float aspectRatio = static_cast<float>(imageWidth_) / static_cast<float>(imageHeight_);
    float theta = degreesToRadians(vFovDegrees);
    float viewportHeight = std::tan(theta / 2) * 2.0f;
    float viewportWidth = viewportHeight * aspectRatio;

    w_ = (origin_ - lookAt).normalized(); // camera backward
    u_ = cross(vUp, w_).normalized(); // camera right
    v_ = cross(w_, u_); // camera up

    Point3 viewportCenter = origin_ - w_; // move viewport plane forward focalLength units along −w vector
    horizontal_ = u_ * viewportWidth;
    vertical_ = v_ * viewportHeight;
    lowerLeft_ = viewportCenter - horizontal_ / 2 - vertical_ / 2;
}

Ray Camera::shootRay(int x, int y, RNG& rng) const {
    // Map randomly sampled point within pixel(x, y) to normalized 3D coordinate inside viewport
    float sx = (x + rng.uniform01()) / (imageWidth_ - 1); // 0 (left) to 1 (right)
    float sy = (imageHeight_ - 1 - y + rng.uniform01()) / (imageHeight_ - 1); // 1 (top) to 0 (bottom) 

    Point3 viewportPoint = lowerLeft_ + sx * horizontal_ + sy * vertical_;
    Vec3 direction = viewportPoint - origin_;

    // Pinhole camera
    if (aperture_ <= 0.0f) {
        return Ray{origin_, direction};
    }

    // Depth of field
    Point3 focusPoint = origin_ + focusDistance_ * direction.normalized();
    Vec3 lensOffset = randomPointOnUnitDisk(rng) * (aperture_ / 2.0f);
    Vec3 offset = u_ * lensOffset.x + v_ * lensOffset.y;
    
    return Ray{origin_ + offset, focusPoint - (origin_ + offset)};
}