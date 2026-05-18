#pragma once

class Chunk {
    public:
        static const int SIZE = 16;

        
        int id;

        Chunk();

        int getBlock(int x, int y) const;
        void setBlock(int x, int y, int type);

        int getChunkX() const;
        int getChunkY() const;

    private:
        int m_chunkX;
        int m_chunkY;

        int grid[SIZE][SIZE];
};

