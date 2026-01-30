#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <random>
#include <filesystem>
#include "math/Vec3.h"
#include "math/Ray.h"
#include "geometry/Sphere.h"
#include "geometry/HittableList.h"
#include "materials/Material.h"
#include "materials/Lambertian.h"
#include "renderer/RayColor.h"
#include "renderer/Camera.h"

int main() {
    // Image
    const int imageWidth = 640;
    const int imageHeight = 480;
    const int samplesPerPixel = 50;
    const int maxDepth = 5;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    // Scene
    HittableList world;

    // Materials
    Lambertian green(Vec3(0.1, 0.8, 0.1));
    Lambertian red(Vec3(0.8, 0.1, 0.1));
    Lambertian blue(Vec3(0.1, 0.1, 0.8));
    Lambertian yellow(Vec3(0.8, 0.8, 0.1));
    Lambertian cyan(Vec3(0.1, 0.8, 0.8));

    // Spheres
    world.add(std::make_shared<Sphere>(Vec3(0,-100.5,-1), 100, &green)); // ground
    world.add(std::make_shared<Sphere>(Vec3(0, 0, -2), 0.5, &red));
    world.add(std::make_shared<Sphere>(Vec3(-1, 0, -1), 0.5, &blue));
    world.add(std::make_shared<Sphere>(Vec3(1, 0, -1), 0.5, &yellow));
    world.add(std::make_shared<Sphere>(Vec3(0, 1, -1), 0.5, &cyan));

    // Camera
    Camera camera{
        Vec3{-2, 2, 2},
        Vec3{0, 0, 0},
        Vec3{0, 1, 0},
        imageWidth, 
        imageHeight, 
        60.0
    };
    // Output to file
    std::filesystem::path outputPath = std::filesystem::current_path() / "output.ppm";
    std::ofstream outFile(outputPath);
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << outputPath << std::endl;
        return 1;
    }
    
    std::cerr << "Writing to " << outputPath << std::endl;

    // Output PPM header
    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    // Loop over pixels
    for (int j = imageHeight-1; j >= 0; --j) {
        for (int i = 0; i < imageWidth; ++i) {
            Vec3 pixelColor(0,0,0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                Ray r = camera.shootRay(i, j);
                pixelColor += rayColor(r, world, maxDepth);
            }
            // Average samples and gamma correction
            double scale = 1.0 / samplesPerPixel;
            double r = std::sqrt(pixelColor.x * scale);
            double g = std::sqrt(pixelColor.y * scale);
            double b = std::sqrt(pixelColor.z * scale);

            outFile << static_cast<int>(256 * std::clamp(r,0.0,0.999)) << ' '
                    << static_cast<int>(256 * std::clamp(g,0.0,0.999)) << ' '
                    << static_cast<int>(256 * std::clamp(b,0.0,0.999)) << '\n';
        }
    }    
    outFile.close();
    return 0;
}
