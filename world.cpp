#include "world.h"
#include <random>
#include <vector>

struct TreeData {
    int x;
    int groundY;
    int height;
};

World::World() {
    for (int x = 0; x < TOTAL_WIDTH; ++x) {
        for (int y = 0; y < TOTAL_HEIGHT; ++y) {
            int chunkX = x / CHUNK_SIZE;
            int chunkY = y / CHUNK_SIZE;
            int localX = x % CHUNK_SIZE;
            int localY = y % CHUNK_SIZE;
            m_chunks[chunkX][chunkY].setBlock(localX, localY, 0);
        }
    }
} 

void World::generate() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> grassHeight(100, 110);
    std::uniform_int_distribution<> stoneHeight(130, 160);
    std::uniform_int_distribution<> deepSlateHeight(161, 185);
    std::uniform_int_distribution<> bedrockHeight(186, TOTAL_HEIGHT - 1);
    std::uniform_int_distribution<> variation(0, 2);
    std::uniform_int_distribution<> trees(0, 10);
    std::uniform_int_distribution<> treeHeight(4, 7);
    std::uniform_int_distribution<> leafHeight(3, 6);


    int height = grassHeight(gen);
    int stone = stoneHeight(gen);
    int deep = deepSlateHeight(gen);
    int bedrock = bedrockHeight(gen);
    std::vector<TreeData> treesToPlace;

    for (int x = 0; x < TOTAL_WIDTH; ++x) {

        for (int y = 0; y < TOTAL_HEIGHT; ++y) {
            int chunkX = x / CHUNK_SIZE;
            int chunkY = y / CHUNK_SIZE;
            int localX = x % CHUNK_SIZE;
            int localY = y % CHUNK_SIZE;

            m_chunks[chunkX][chunkY].setBlock(localX, localY, 0);
        }

        int chunkX = x / CHUNK_SIZE;
        int gchunkY = height / CHUNK_SIZE;
        int localX = x % CHUNK_SIZE;
        int glocalY = height % CHUNK_SIZE;

        m_chunks[chunkX][gchunkY].setBlock(localX, glocalY, 1);
        if(trees(gen) == 0) {
            treesToPlace.push_back({x, height, treeHeight(gen)});
        }

        for (int y = height + 1; y < stone; ++y) {
            int chunkY = y / CHUNK_SIZE;
            int localY = y % CHUNK_SIZE;
            m_chunks[chunkX][chunkY].setBlock(localX, localY, 2);
        }
        for (int y = stone; y < deep; ++y) {
            int chunkY = y / CHUNK_SIZE;
            int localY = y % CHUNK_SIZE;
            m_chunks[chunkX][chunkY].setBlock(localX, localY, 3);
        }
        for (int y = deep; y < bedrock; ++y) {
            int chunkY = y / CHUNK_SIZE;
            int localY = y % CHUNK_SIZE;
            m_chunks[chunkX][chunkY].setBlock(localX, localY, 4);
        }
        for (int y = bedrock; y < TOTAL_HEIGHT; ++y) {
            int chunkY = y / CHUNK_SIZE;
            int localY = y % CHUNK_SIZE;
            m_chunks[chunkX][chunkY].setBlock(localX, localY, 5);
        }

        int r = variation(gen);
        if (r == 0 && height > 0) --height;
        else if (r == 2 && height < TOTAL_HEIGHT - 1) ++height;

        int r2 = variation(gen);
        if (r2 == 0 && stone > height + 3) --stone;
        else if (r2 == 2 && stone < TOTAL_HEIGHT - 1) ++stone;

        int r3 = variation(gen);
        if (r3 == 0 && deep > stone + 3) --deep;
        else if (r3 == 2 && deep < TOTAL_HEIGHT - 1) ++deep;

        int r4 = variation(gen);
        if (r4 == 0 && bedrock > deep + 3) --bedrock;
        else if (r4 == 2 && bedrock < TOTAL_HEIGHT - 1) ++bedrock;
    }

    for(const TreeData& tree : treesToPlace) {
        int topOfTree = tree.groundY - tree.height;

        for(int y = tree.groundY - 1; y > topOfTree; --y) {
            setBlock(tree.x, y, 7);
        }

        
        for(int i = -2; i <= 2; ++i) {
            setBlock(tree.x + i, topOfTree, 8);
        }
        for(int i = -1; i <= 1; ++i) {
            setBlock(tree.x + i, topOfTree - 1, 8);
        }
        for(int i = -1; i <= 1; ++i) {
            setBlock(tree.x + i, topOfTree - 2, 8);
        }
        setBlock(tree.x, topOfTree - 3, 8);
    }
}

int World::getBlock(int x, int y) const {
    if (x < 0 || x >= TOTAL_WIDTH || y < 0 || y >= TOTAL_HEIGHT)
        return 0;

    int chunkX = x / CHUNK_SIZE;
    int chunkY = y / CHUNK_SIZE;
    int localX = x % CHUNK_SIZE;
    int localY = y % CHUNK_SIZE;

    return m_chunks[chunkX][chunkY].getBlock(localX, localY);
}

void World::setBlock(int x, int y, int type) {
    if (x < 0 || x >= TOTAL_WIDTH || y < 0 || y >= TOTAL_HEIGHT)
        return;

    int chunkX = x / CHUNK_SIZE;
    int chunkY = y / CHUNK_SIZE;
    int localX = x % CHUNK_SIZE;
    int localY = y % CHUNK_SIZE;

    m_chunks[chunkX][chunkY].setBlock(localX, localY, type);
}


bool World::isSolid(int type) const {
    switch (type) {
        case 0: 
            return false;
        default:
            return true;
    }
}
sf::FloatRect World::getBlockHitbox(int x, int y, float tileSize) const {
    if(getBlock(x, y) == 20) {
        return sf::FloatRect(
        sf::Vector2f(static_cast<float>(x) * tileSize, (static_cast<float>(y) * tileSize) + tileSize / 2),
        sf::Vector2f(tileSize, tileSize / 2)
    );

    }
    return sf::FloatRect(
        sf::Vector2f(static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize),
        sf::Vector2f(tileSize, tileSize)
    );
}
