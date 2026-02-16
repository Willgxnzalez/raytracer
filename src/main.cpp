#include <iostream>
#include <memory>
#include "core/Vec3.h"
#include "core/Ray.h"
#include "geometry/Sphere.h"
#include "renderer/Camera.h"
#include "renderer/Scene.h"
#include "renderer/Renderer.h"
#include "util/RNG.h"


int main() {
    // Image
    const int imageWidth = 800;
    const int imageHeight = 640;
    const int samplesPerPixel = 100;
    const int maxDepth = 10;

    // Scene
    Scene world;

    // Materials
    int ground = world.addDiffuse(Vec3(0.5f, 0.5f, 0.5f));
    int red =  world.addDiffuse(Vec3(0.8f, 0.1f, 0.1f));
    int blue = world.addDiffuse(Vec3(0.1f, 0.1f, 0.8f));
    int yellow = world.addDiffuse(Vec3(0.8f, 0.8f, 0.1f));
    int green = world.addDiffuse(Vec3(0.1f, 0.8f, 0.1f));
    int cyan = world.addDiffuse(Vec3(0.1f, 0.7f, 0.9f));
    int purple = world.addDiffuse(Vec3(0.6f, 0.1f, 0.8f));
    int orange = world.addDiffuse(Vec3(0.9f, 0.5f, 0.1f));
    int brown = world.addDiffuse(Vec3(0.6f, 0.4f, 0.2f));

    int colors[] = {red, blue, yellow, green, cyan, purple, orange};
    int glass = world.addDielectric(1.5f);

    // Ground plane
    world.addSphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, ground);

    // Sphere(Vec3(-0.5f, 1.0f, -3.0f), 1.0f, &);      // Left - matte
    world.addSphere(Vec3(0.0f, 1.0f, -2.0f), 1.0f, brown);      // Center - matte
    world.addSphere(Vec3(0.5f, 1.0f, -1.0f), 1.0f, glass);       // Right - glass

    // Random small spheres scattered on ground
    RNG rng{42};
    for (int i = 0; i < 100; ++i) {
        Vec3 center(
            rng.uniform(-5.0f, 5.0f),
            0.4f,  // Small elevation
            rng.uniform(-7.5f, 2.0f)
        );
        world.addSphere(center, 0.4f, colors[rng.uniformInt(0, 7)]);
    }
    world.build();

    // Camera setup - lower angle looking slightly up
    Point3 cameraPosition{0.0f, 1.8f, 5.0f};  // Lower, closer
    Point3 focusTarget{0.0f, 0.8f, 1.0f};    // Looking at hero spheres

    Camera camera{
        cameraPosition,
        focusTarget,
        Vec3{0.0f, 1.0f, 0.0f},
        imageWidth, 
        imageHeight, 
        50.0f,           // FOV
        0.1f,            // Aperture for depth of field
        6.0f             // Focus distance
    };

    Renderer renderer{imageWidth, imageHeight, samplesPerPixel, 64};
    renderer.render(camera, world, "renders/output.ppm");
    
    return EXIT_SUCCESS;
}
