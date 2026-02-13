#pragma once

#include "core/Vec3.h"
#include <vector>

class Film {
    std::vector<Color> pixels;
    Film(int imageWidth, int imageHeight);

    void colorPixel(int x, int y, Vec3 color);
    void output(const char* path);
};