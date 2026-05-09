#include "chunk.h"

Chunk::Chunk()
    : m_chunkX(0), m_chunkY(0)
{
    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            grid[x][y] = BlockType::Air;
        }
    }
}

Chunk::BlockType Chunk::getBlock(int x, int y) const {
    return grid[x][y];
}

void Chunk::setBlock(int x, int y, BlockType type) {
    grid[x][y] = type;
}

int Chunk::getChunkX() const {
    return m_chunkX;
}

int Chunk::getChunkY() const {
    return m_chunkY;
}
