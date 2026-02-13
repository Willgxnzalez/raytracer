#pragma once

#include "core/Vec3.h"
#include <vector>

class Film {
    std::vector<Color> pixels;
    int width_, height_;

public:
    Film(int width, int height) {}

    void colorPixel(int x, int y, const Color&  color);
    void output(const char* path);
};