#pragma once

#include "core/Vec3.h"
#include <vector>
#include <string>

class Film {
public:
    Film(int imageWidth, int imageHeight);
    
    void colorPixel(int x, int y, const Color&  color);
    void output(const std::string& path) const;
    
private:
    std::vector<Color> pixels_;
    int width_, height_;
};