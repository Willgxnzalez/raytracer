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

Camera::Camera(
    const Vec3& lookFrom, 
    const Vec3& lookAt, 
    const Vec3& vUp, 
    int imageWidth, 
    int imageHeight, 
    double vFovDegrees
)
    : origin_(lookFrom), imageWidth_(imageWidth), imageHeight_(imageHeight)
{
    // Using Tan(vFov/2) = (viewportHeight/2) / focalLength
    double aspectRatio = static_cast<double>(imageWidth_) / static_cast<double>(imageHeight_);
    double theta = degreesToRadians(vFovDegrees);
    double viewportHeight = std::tan(theta / 2) * focalLength_ * 2.0;
    double viewportWidth = viewportHeight * aspectRatio;

    w_ = (origin_ - lookAt).normalized(); // camera backward
    u_ = cross(vUp, w_).normalized(); // camera right
    v_ = cross(w_, u_).normalized(); // camera up

    Vec3 viewportCenter = origin_ - focalLength_ * w_; // move viewport plane forward focalLength units along −w vector
    horizontal_ = u_ * viewportWidth;
    vertical_ = v_ * viewportHeight;
    lowerLeft_ = viewportCenter - horizontal_ / 2 - vertical_ / 2;
}

Ray Camera::shootRay(int i, int j) const {
    // Map randomly sampled point within pixel(i, j) to normalized 3D coordinate inside viewport
    double u = (i + randomOffset()) / (imageWidth_ - 1); // 0 (left edge) <= u <= 1 (right edge)
    double v = (j + randomOffset()) / (imageHeight_ - 1); // 0 (bottom edge) <= v <= 1 (top edge)

    Vec3 viewportPoint = lowerLeft_ + u*horizontal_ + v*vertical_;
    Vec3 rayDirection = viewportPoint - origin_;
    return Ray{origin_, rayDirection};
}