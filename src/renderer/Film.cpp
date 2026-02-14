#include "renderer/Film.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>

Film::Film(int imageWidth, int imageHeight) : 
    width_(imageWidth), 
    height_(imageHeight), 
    pixels_(imageWidth * imageHeight)
{}

void Film::colorPixel(int x, int y, const Color &color) {
    pixels_[y * width_ + x] = color;
}

void Film::output(const std::string& path) const {
    std::filesystem::path filePath = std::filesystem::current_path() / path;
    if (filePath.has_parent_path()) {
        std::filesystem::create_directories(filePath.parent_path());
    }
    std::ofstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << path << std::endl;
        return;
    }
    
    // PPM header
    file << "P3\n" << width_ << " " << height_ << "\n255\n";
    
    // Write pixels with gamma correction
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Color linear = pixels_[y * width_ + x];
            
            // Gamma correct (sqrt for gamma 2.0)
            float r = std::sqrt(linear.x);
            float g = std::sqrt(linear.y);
            float b = std::sqrt(linear.z);
            
            // Clamp and convert to [0, 255]
            file << static_cast<int>(256 * std::clamp(r, 0.0f, 0.999f)) << ' '
                 << static_cast<int>(256 * std::clamp(g, 0.0f, 0.999f)) << ' '
                 << static_cast<int>(256 * std::clamp(b, 0.0f, 0.999f)) << '\n';
        }
    }
    file.close();
}