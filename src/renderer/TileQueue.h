#pragma once

#include <atomic>
#include <vector>

struct Tile {
    int x0, y0;
    int x1, y1;
};

class TileQueue {
    std::atomic<int> index;
    std::vector<Tile> tiles;

public:
    TileQueue(int imageWidth, int imageHeight, int tileSize);
    bool next(Tile& tile);
};