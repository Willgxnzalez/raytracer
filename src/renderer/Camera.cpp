#include "renderer/Camera.h"
#include "math/Ray.h"
#include <random>
#include <numbers>
#include <cmath>
#include <iostream>

double degreesToRadians(double degrees) {
    return degrees * (std::numbers::pi / 180);
}

double randomOffset() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

Vec3 randomPointOnUnitDisk() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<double> angleDis(0.0, 2.0 * std::numbers::pi);
    static std::uniform_real_distribution<double> radiusDis(0.0, 1.0); 
    double angle = angleDis(gen);
    double radius = std::sqrt(radiusDis(gen));
    return Vec3{std::cos(angle) * radius, std::sin(angle) * radius, 0};
}

Camera::Camera(
    const Vec3& lookFrom, 
    const Vec3& lookAt, 
    const Vec3& vUp, 
    int imageWidth, 
    int imageHeight, 
    double vFovDegrees,
    double aperture,
    double focusDistance
) :
    origin_(lookFrom), 
    imageWidth_(imageWidth), 
    imageHeight_(imageHeight), 
    aperture_(aperture), 
    focusDistance_(focusDistance)
{
    // Using Tan(vFov/2) = (viewportHeight/2) / 1 (focalLength)
    double aspectRatio = static_cast<double>(imageWidth_) / static_cast<double>(imageHeight_);
    double theta = degreesToRadians(vFovDegrees);
    double viewportHeight = std::tan(theta / 2) * 2.0;
    double viewportWidth = viewportHeight * aspectRatio;

    w_ = (origin_ - lookAt).normalized(); // camera backward
    u_ = cross(vUp, w_).normalized(); // camera right
    v_ = cross(w_, u_); // camera up

    Vec3 viewportCenter = origin_ - w_; // move viewport plane forward focalLength units along −w vector
    horizontal_ = u_ * viewportWidth;
    vertical_ = v_ * viewportHeight;
    lowerLeft_ = viewportCenter - horizontal_ / 2 - vertical_ / 2;
}

Ray Camera::shootRay(int i, int j) const {
    // Map randomly sampled point within pixel(i, j) to normalized 3D coordinate inside viewport
    double px = (i + randomOffset()) / (imageWidth_ - 1); // 0 (left) to 1 (right)
    double py = (imageHeight_ - 1 - j + randomOffset()) / (imageHeight_ - 1); // 1 (top edge) to 0 (bottom edge)

    Vec3 viewportPoint = lowerLeft_ + px * horizontal_ + py * vertical_;
    Vec3 direction = viewportPoint - origin_;
    Vec3 focusPoint = origin_ + focusDistance_ * direction.normalized(); // Point on focus plane
    Vec3 lensOffset = randomPointOnUnitDisk() * (aperture_ / 2);
    Vec3 offset = u_ * lensOffset.x + v_ * lensOffset.y;
    Vec3 rayOrigin = origin_ + offset;
    Vec3 rayDirection = focusPoint - rayOrigin;

    return Ray{rayOrigin, rayDirection};
}