#include "Camera.h"
#include <random>
#include <numbers>
#include <cmath>
#include <iostream>

float degreesToRadians(float degrees) {
    return degrees * (std::numbers::pi / 180);
}

float randomOffset() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

Point3 randomPointOnUnitDisk() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> angleDis(0.0f, 2.0f * std::numbers::pi);
    static std::uniform_real_distribution<float> radiusDis(0.0f, 1.0f); 
    float angle = angleDis(gen);
    float radius = std::sqrt(radiusDis(gen));
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

Ray Camera::shootRay(int i, int j) const {
    // Map randomly sampled point within pixel(i, j) to normalized 3D coordinate inside viewport
    float px = (i + randomOffset()) / (imageWidth_ - 1); // 0 (left) to 1 (right)
    float py = (imageHeight_ - 1 - j + randomOffset()) / (imageHeight_ - 1); // 1 (top edge) to 0 (bottom edge)

    Point3 viewportPoint = lowerLeft_ + px * horizontal_ + py * vertical_;
    Vec3 direction = viewportPoint - origin_;
    Point3 focusPoint = origin_ + focusDistance_ * direction.normalized(); // Point on focus plane
    Vec3 lensOffset = randomPointOnUnitDisk() * (aperture_ / 2);
    Vec3 offset = u_ * lensOffset.x + v_ * lensOffset.y;
    Point3 rayOrigin = origin_ + offset;
    Vec3 rayDirection = focusPoint - rayOrigin;

    return Ray{rayOrigin, rayDirection};
}