#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <filesystem>
#include <atomic>
#include "core/Vec3.h"
#include "core/Ray.h"
#include "geometry/Sphere.h"
#include "materials/Material.h"
#include "materials/Lambertian.h"
#include "materials/Dielectric.h"
#include "renderer/TraceRay.h"
#include "renderer/Camera.h"
#include "renderer/Scene.h"
#include "util/ProgressBar.h"

extern std::atomic<uint64_t> gSphereHits;
extern std::atomic<uint64_t> gAABBHits;

int main() {
    // Image
    const int imageWidth = 840;
    const int imageHeight = 600;
    const int samplesPerPixel = 50;
    const int maxDepth = 5;
    const int barWidth = 40;

    // Scene
    Scene world;

    // Materials
    Lambertian grey(Vec3(0.3f, 0.3f, 0.3f));
    Lambertian red(Vec3(0.8f, 0.1f, 0.1f));
    Lambertian blue(Vec3(0.1f, 0.1f, 0.8f));
    Lambertian yellow(Vec3(0.8f, 0.8f, 0.1f));
    Lambertian cyan(Vec3(0.1f, 0.8f, 0.8f));
    
    Dielectric glass(1.5);

    // Spheres
    world.add(std::make_shared<Sphere>(Vec3(0.0f, -100.5f, 9.0f), 100.0f, &grey)); // ground
    world.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, -1.0f), 0.5f, &red));
    world.add(std::make_shared<Sphere>(Vec3(-1.0f, 0.0f, 0.0f), 0.5f, &glass));
    world.add(std::make_shared<Sphere>(Vec3(1.0f, 0.0f, 0.0f), 0.5f, &yellow));
    world.add(std::make_shared<Sphere>(Vec3(0.0f, 1.0f, 0.0f), 0.5f, &glass));

    // for (int x = -50; x <= 50; ++x) {
    //     for (int z = -50; z <= 50; ++z) {
    //         world.add(std::make_shared<Sphere>(
    //             Vec3{x * 2.5, 0.2, z * 2.5},
    //             0.2,
    //             &blue
    //         ));
    //     }
    // }

    world.build();

    // Cyan
    Vec3 cameraPosition{-2, 2, 2};
    Vec3 focusTarget{0, 1, 0};
    float focusDistance = (focusTarget - cameraPosition).length();

    // Camera
    Camera camera{
        cameraPosition,
        focusTarget,
        Vec3{0, 1, 0},
        imageWidth, 
        imageHeight, 
        75.0f,
        0.3f,
        focusDistance // For testing, this is distance from the camera to the target object
    };
    // Output to file
    std::filesystem::path outputPath = std::filesystem::current_path() / "output.ppm";
    std::ofstream outFile(outputPath);
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << outputPath << std::endl;
        return EXIT_FAILURE;
    }
    
    std::clog << "Writing to " << outputPath << std::endl;
    ProgressBar bar(std::clog, imageHeight);

    RNG rng{1111};

    // Output PPM header
    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";


    // Loop over pixels
    for (int j = 0; j < imageHeight; ++j) {
        bar.update();
        for (int i = 0; i < imageWidth; ++i) {
            Color pixelColor(0.0f ,0.0f ,0.0f);
            for (int s = 0; s < samplesPerPixel; ++s) { // Monte Carlo Sampling
                Ray r = camera.shootRay(i, j, rng);
                pixelColor += traceRay(r, world, rng, maxDepth);
            }
            // Average samples and gamma correction
            float scale = 1.0f / samplesPerPixel;
            float r = std::sqrt(pixelColor.x * scale);
            float g = std::sqrt(pixelColor.y * scale);
            float b = std::sqrt(pixelColor.z * scale);

            outFile << static_cast<int>(256 * std::clamp(r,0.0f,0.999f)) << ' '
                    << static_cast<int>(256 * std::clamp(g,0.0f,0.999f)) << ' '
                    << static_cast<int>(256 * std::clamp(b,0.0f,0.999f)) << '\n';
        }
    }
    std::cout << "AABB tests:   " << gAABBHits   << "\n";
    std::cout << "Sphere tests: " << gSphereHits << "\n";
    outFile.close();
    return EXIT_SUCCESS;
}
