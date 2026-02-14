#include "renderer/Renderer.h"
#include "renderer/TraceRay.h"
#include "core/Vec3.h"
#include "util/RNG.h"
#include <thread>
#include <vector>

using namespace std::chrono;

Renderer::Renderer(int imageWidth, int imageHeight, int samplesPerPixel, int tileSize, int maxDepth) :   
    imageWidth_(imageWidth), 
    imageHeight_(imageHeight),
    samplesPerPixel_(samplesPerPixel),
    tileSize_(tileSize),
    maxDepth_(maxDepth),
    film_(imageWidth, imageHeight),
    queue_(imageWidth, imageHeight, tileSize) 
{}

void Renderer::render(const Camera& camera, const Scene& scene, const std::string& path) {
    
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(
            &Renderer::renderWorker,
            this, // Reference to object the non-static method belongs to
            i, 
            std::cref(camera),
            std::cref(scene)
        );
    }

    for (auto& t : threads)
        t.join();

    film_.output(path);
}

void Renderer::renderWorker(int threadId, const Camera& camera, const Scene& scene) {
    RNG rng{globalSeed_, threadId};

    Tile tile;
    while(queue_.next(tile)) {
        for (int y = tile.y0; y < tile.y1; ++y) {
            for (int x = tile.x0; x < tile.x1; ++x) {
                Color pixelColor(0.0f, 0.0f, 0.0f);
                for (int s = 0; s < samplesPerPixel_; ++s) {
                    Ray r = camera.shootRay(x, y, rng);
                    pixelColor += traceRay(r, scene, rng, maxDepth_);
                }

                pixelColor /= static_cast<float>(samplesPerPixel_);

                film_.colorPixel(x, y, pixelColor);
            }
        }
    }
}