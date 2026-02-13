#include "renderer/TileQueue.h"


TileQueue::TileQueue(int imageWidth, int imageHeight, int tileSize) : index(0) {
    for (int y = 0; y < imageHeight; y += tileSize) {
        for (int x = 0; x < imageWidth; x += tileSize) {
            tiles.push_back({
                x,
                y,
                std::min(x + tileSize, imageWidth),
                std::min(y + tileSize, imageHeight)
            });
        }
    }
}

bool TileQueue::next(Tile& tile) {
    int idx = index.fetch_add(1, std::memory_order_relaxed);
    if (idx >= tiles.size()) return false;
    tile = tiles[idx];
    return true;
}