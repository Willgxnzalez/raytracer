#pragma once

#include "renderer/Camera.h"
#include "renderer/Film.h"
#include "renderer/TileQueue.h"
#include "renderer/Scene.h"
#include <cstdint>
#include <string>

class Renderer {
public: 
    Renderer(
        int imageWidth, 
        int imageHeight, 
        int samplesPerPixel, 
        int tileSize = 32, 
        int maxDepth = 5
    );

    void render(const Camera& camera, const Scene& scene, const std::string& path);
    void renderWorker(int threadId, const Camera& camera, const Scene& scene);

private:
    int imageWidth_, imageHeight_;
    int samplesPerPixel_;
    int tileSize_;
    int maxDepth_;

    Film film_;
    TileQueue queue_;

    const uint64_t globalSeed_ = 1215;
};