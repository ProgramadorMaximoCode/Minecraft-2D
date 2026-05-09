#pragma once

class Chunk {
    public:
        static const int SIZE = 16;

        enum class BlockType {
            Air,
            Dirt,
            Grass,
            Stone,
            DeepSlate,
            Bedrock
        };

        Chunk();

        BlockType getBlock(int x, int y) const;
        void setBlock(int x, int y, BlockType type);

        int getChunkX() const;
        int getChunkY() const;

    private:
        int m_chunkX;
        int m_chunkY;

        BlockType grid[SIZE][SIZE];
};

