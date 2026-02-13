#pragma once

#include "renderer/Camera.h"
#include "renderer/Film.h"
#include "renderer/TileQueue.h"
#include "renderer/Scene.h"
#include <cstdint>

class Renderer {
    int imageWidth_, imageHeight_;
    int samplesPerPixel_;
    int tileSize_;

    Film film_;
    TileQueue queue_;

    const uint64_t globalSeed_ = 1215;

public: 
    Renderer(int imageWidth, int imageHeight, int samplesPerPixel, int tileSize = 16);

    void render(const Camera& camera, const Scene& scene);
    void renderWorker(int threadId, const Camera& camera, const Scene& scene);
};