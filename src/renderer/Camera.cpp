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

Camera::Camera(int _imageWidth, int _imageHeight, double vFov)
    : imageWidth(_imageWidth), imageHeight(_imageHeight), focalLength(1.0)
{
    double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
    double theta = degreesToRadians(vFov);
    double viewportHeight = std::tan(theta / 2) * focalLength * 2.0;
    double viewportWidth = viewportHeight * aspectRatio;

    horizontal = Vec3(viewportWidth, 0, 0);
    vertical = Vec3(0, viewportHeight, 0);
    lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
}

Ray Camera::shootRay(int i, int j) const {
    // Map pixel to normalized 3D coordinate inside viewport rectangle
    double u = (i + randomOffset()) / (imageWidth-1); // 0 (left edge) <= u <= 1 (right edge)
    double v = (j + randomOffset()) / (imageHeight-1); // 0 (bottom edge) <= u <= 1 (top edge)

    Vec3 pointOnViewport = lowerLeftCorner + u*horizontal + v*vertical;
    return Ray{origin, pointOnViewport - origin}; // ray direction is pointOnViewport - origin = origin -> pointOnViewport
}