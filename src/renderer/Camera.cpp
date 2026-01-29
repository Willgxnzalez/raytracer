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
    int _imageWidth, 
    int _imageHeight, 
    double vFovDegrees
)
    : origin(lookFrom), imageWidth(_imageWidth), imageHeight(_imageHeight), focalLength(1.0)
{
    double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    double theta = degreesToRadians(vFovDegrees);
    double viewportHeight = std::tan(theta / 2) * focalLength * 2.0;
    double viewportWidth = viewportHeight * aspectRatio;

    // Compute camera basis vectors
    w = (lookFrom - lookAt).normalized(); // camera backward
    u = cross(vUp, w).normalized(); // camera right
    v = cross(w, u).normalized(); // camera up

    Vec3 center = origin - focalLength * w; // move viewport plane forward focalLength units along −w vector
    horizontal = u * viewportWidth;
    vertical = v * viewportHeight;
    lowerLeftCorner = center - horizontal / 2 - vertical / 2;
    std::cout << "Camera center: " << center << std::endl
              << "horizontal: " << horizontal << std::endl
              << "vertical: " << vertical << std::endl
              << "lowerLeftCorner: " << lowerLeftCorner << std::endl;
}

Ray Camera::shootRay(int i, int j) const {
    // Map randomly sampled point with pixel(i, j) to normalized 3D coordinate inside viewport rectangle
    double u = (i + randomOffset()) / (imageWidth-1); // 0 (left edge) <= u <= 1 (right edge)
    double v = (j + randomOffset()) / (imageHeight-1); // 0 (bottom edge) <= v <= 1 (top edge)

    Vec3 pointOnViewport = lowerLeftCorner + u*horizontal + v*vertical;
    return Ray{origin, pointOnViewport - origin}; // ray direction is pointOnViewport - origin = origin -> pointOnViewport
}