#pragma once
#include <SFML/Graphics.hpp>
#include "chunk.h"

class World {
    public:
        static const int CHUNK_SIZE = Chunk::SIZE;

        static const int WORLD_WIDTH = 63;
        static const int WORLD_HEIGHT = 14;

        static const int TOTAL_WIDTH = CHUNK_SIZE * WORLD_WIDTH;
        static const int TOTAL_HEIGHT = CHUNK_SIZE * WORLD_HEIGHT;

        World();
        void generate();

        int getBlock(int x, int y) const;
        void setBlock(int x, int y, int type);
        bool isSolid(int type) const; 
        sf::FloatRect getBlockHitbox(int x, int y, float tileSize) const;

    private:
        Chunk m_chunks[WORLD_WIDTH][WORLD_HEIGHT];
};
