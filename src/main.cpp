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
    const int samplesPerPixel = 75;
    const int maxDepth = 8;

    // Scene
    Scene world;

    // Materials
    int ground = world.addDiffuse(Color(0.5f, 0.5f, 0.5f));
    int red =  world.addDiffuse(Color(0.8f, 0.1f, 0.1f));
    int blue = world.addDiffuse(Color(0.1f, 0.1f, 0.8f));
    int yellow = world.addDiffuse(Color(0.8f, 0.8f, 0.1f));
    int green = world.addDiffuse(Color(0.1f, 0.8f, 0.1f));
    int cyan = world.addDiffuse(Color(0.1f, 0.7f, 0.9f));
    int purple = world.addDiffuse(Color(0.6f, 0.1f, 0.8f));
    int orange = world.addDiffuse(Color(0.9f, 0.5f, 0.1f));
    int brown = world.addDiffuse(Color(0.6f, 0.4f, 0.2f));
    int colors[] = {red, blue, yellow, green, cyan, purple, orange};

    int glass = world.addDielectric(1.5f);

    int gold = world.addMetal(Color(1.0, 0.78, 0.34));

    int light = world.addEmissive(Color(1.0f));

    int plastic = world.addPhysical(Color(0.4f, 0.4f, 0.4f), 0.3f, 0.3f );

    world.addSphere(Color(0.0f, -1000.0f, 0.0f), 1000.0f, ground);

    world.addSphere(Point3(-0.6f, 1.0f, -3.0f), 1.0f, glass);
    world.addSphere(Point3(0.0f, 1.0f, -2.0f), 1.0f, brown);
    world.addSphere(Point3(0.6f, 1.0f, -1.0f), 1.0f, gold);
    world.addSphere(Point3(1.2f, 1.0f, 0.0f), 1.0f, plastic);

    world.addSphere(Point3(0.0f, 3.0f, -0.5f), 0.3f, light);

    // Hero sphere positions
    Point3 heroPositions[] = {
        Point3(-0.6f, 1.0f, -3.0f),
        Point3(0.0f, 1.0f, -2.0f),
        Point3(0.6f, 1.0f, -1.0f),
        Point3(1.2f, 1.0f, 0.0f)
    };
    
    RNG rng{42};
    for (int i = 0; i < 100; ++i) {
        Point3 heroCenter = heroPositions[rng.uniformInt(0, 4)];
        
        // Generate position in a ring around the random hero sphere
        float angle = rng.uniform(0.0f, 2.0f * 3.14159f);
        float distance = rng.uniform(2.5f, 6.0f);
        
        Vec3 offset(
            distance * std::cos(angle),
            0.0f,
            distance * std::sin(angle)
        );
        
        Point3 center = heroCenter + offset;
        center.y = 0.25f;  // Keep on ground
        
        world.addSphere(center, 0.25f, colors[rng.uniformInt(0, 7)]);
    }
    world.build();

    // Camera setup - lower angle looking slightly up
    Point3 cameraPosition{0.0f, 1.8f, 5.0f};  // Lower, closer
    Point3 focusTarget{0.0f, 0.8f, -1.0f};    // Looking at hero spheres
    float focusDistance = (focusTarget - cameraPosition).length();

    Camera camera{
        cameraPosition,
        focusTarget,
        Vec3{0.0f, 1.0f, 0.0f},
        imageWidth, 
        imageHeight, 
        50.0f,           // FOV
        0.0f,            // Aperture for depth of field
        focusDistance
    };

    Renderer renderer{imageWidth, imageHeight, samplesPerPixel};
    renderer.render(camera, world, "renders/output.ppm");
    
    return EXIT_SUCCESS;
}
