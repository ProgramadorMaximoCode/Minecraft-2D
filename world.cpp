#include "world.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

struct TreeData {
    int x;
    int groundY;
    int height;
};

struct CaveTunnel {
    float x;
    float y;
    float angle;
    float radius;
    int length;
    int branchesLeft;
};

namespace {
    bool canCarveCave(int block) {
        return block == 3 || block == 4;
    }

    void carveCaveCircle(World& world, int centerX, int centerY, int radiusX, int radiusY) {
        for(int dx = -radiusX; dx <= radiusX; ++dx) {
            for(int dy = -radiusY; dy <= radiusY; ++dy) {
                float nx = static_cast<float>(dx) / static_cast<float>(radiusX);
                float ny = static_cast<float>(dy) / static_cast<float>(radiusY);

                if(nx * nx + ny * ny > 1.0f)
                    continue;

                int x = centerX + dx;
                int y = centerY + dy;

                if(y < 118 || y >= World::TOTAL_HEIGHT - 6)
                    continue;

                if(canCarveCave(world.getBlock(x, y)))
                    world.setBlock(x, y, 0);
            }
        }
    }
}

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
    FastNoiseLite terrain;
    FastNoiseLite terrainDetail;
    FastNoiseLite layerNoise;
    FastNoiseLite caveMain;
    FastNoiseLite caveBranch;
    FastNoiseLite caveTunnelCurve;
    FastNoiseLite caveTunnelRadius;

    FastNoiseLite coalOre;
    FastNoiseLite ironOre;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> seedDist(0, 999999999);

    terrain.SetSeed(seedDist(gen));
    terrainDetail.SetSeed(seedDist(gen));
    layerNoise.SetSeed(seedDist(gen));
    caveMain.SetSeed(seedDist(gen));
    caveBranch.SetSeed(seedDist(gen));
    caveTunnelCurve.SetSeed(seedDist(gen));
    caveTunnelRadius.SetSeed(seedDist(gen));
    coalOre.SetSeed(seedDist(gen));
    ironOre.SetSeed(seedDist(gen));

    terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainDetail.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    layerNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveMain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveBranch.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveTunnelCurve.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveTunnelRadius.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    coalOre.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    ironOre.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);


    terrain.SetFrequency(0.01f);
    terrainDetail.SetFrequency(0.05f);
    layerNoise.SetFrequency(0.018f);
    caveMain.SetFrequency(0.028f);
    caveBranch.SetFrequency(0.075f);
    caveTunnelCurve.SetFrequency(0.035f);
    caveTunnelRadius.SetFrequency(0.055f);
    coalOre.SetFrequency(0.18);
    ironOre.SetFrequency(0.2);

    std::uniform_int_distribution<> trees(0, 10);
    std::uniform_int_distribution<> treeHeight(4, 7);
    std::uniform_int_distribution<> tunnelCountDist(28, 46);
    std::uniform_int_distribution<> startXDist(0, TOTAL_WIDTH - 1);
    std::uniform_int_distribution<> startYDist(126, TOTAL_HEIGHT - 24);
    std::uniform_int_distribution<> lengthDist(100, 240);
    std::uniform_int_distribution<> branchChanceDist(0, 99);
    std::uniform_real_distribution<float> angleDist(-0.45f, 0.45f);
    std::uniform_real_distribution<float> radiusDist(1.2f, 2.0f);

    std::vector<TreeData> treesToPlace;

    for (int x = 0; x < TOTAL_WIDTH; ++x) {
        float hillNoise = terrain.GetNoise(static_cast<float>(x), 0.0f);
        float detailNoise = terrainDetail.GetNoise(static_cast<float>(x), 90.0f);
        int height = std::clamp(
            static_cast<int>(std::round(104.0f + hillNoise * 18.0f + detailNoise * 5.0f)),
            70,
            140
        );

        int stone = std::clamp(
            height + 8 + static_cast<int>(std::round((layerNoise.GetNoise(static_cast<float>(x), 40.0f) + 1.0f) * 8.0f)),
            height + 4,
            TOTAL_HEIGHT - 32
        );

        int deep = std::clamp(
            stone + 35 + static_cast<int>(std::round((layerNoise.GetNoise(static_cast<float>(x), 120.0f) + 1.0f) * 12.0f)),
            stone + 4,
            TOTAL_HEIGHT - 12
        );

        int bedrock = std::clamp(
            TOTAL_HEIGHT - 6 + static_cast<int>(std::round(layerNoise.GetNoise(static_cast<float>(x), 220.0f) * 3.0f)),
            deep + 4,
            TOTAL_HEIGHT - 1
        );

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
    }

    for(int x = 0; x < TOTAL_WIDTH; ++x) {
        for(int y = 120; y < TOTAL_HEIGHT - 8; ++y) {
            if(!canCarveCave(getBlock(x, y)))
                continue;

            float chamberNoise = caveMain.GetNoise(static_cast<float>(x), static_cast<float>(y));
            float detailNoise = caveBranch.GetNoise(static_cast<float>(x), static_cast<float>(y));

            if(chamberNoise > 0.64f && detailNoise > -0.20f) {
                int radiusX = 2 + static_cast<int>((detailNoise + 1.0f) * 1.5f);
                int radiusY = 1 + static_cast<int>((chamberNoise + 1.0f) * 0.9f);
                carveCaveCircle(*this, x, y, radiusX, radiusY);
            }
        }
    }

    std::vector<CaveTunnel> tunnels;
    int tunnelCount = tunnelCountDist(gen);
    tunnels.reserve(tunnelCount * 3);

    for(int i = 0; i < tunnelCount; ++i) {
        float direction = (i % 2 == 0) ? 0.0f : 3.1415926f;
        tunnels.push_back({
            static_cast<float>(startXDist(gen)),
            static_cast<float>(startYDist(gen)),
            direction + angleDist(gen),
            radiusDist(gen),
            lengthDist(gen),
            2
        });
    }

    for(size_t i = 0; i < tunnels.size(); ++i) {
        CaveTunnel tunnel = tunnels[i];

        for(int step = 0; step < tunnel.length; ++step) {
            float curve = caveTunnelCurve.GetNoise(tunnel.x, tunnel.y);
            float radiusNoise = caveTunnelRadius.GetNoise(tunnel.x, tunnel.y);
            float radius = std::clamp(tunnel.radius + radiusNoise * 1.2f, 1.4f, 3.2f);

            int radiusX = std::max(1, static_cast<int>(std::round(radius * 1.35f)));
            int radiusY = std::max(1, static_cast<int>(std::round(radius * 0.85f)));
            carveCaveCircle(*this, static_cast<int>(std::round(tunnel.x)), static_cast<int>(std::round(tunnel.y)), radiusX, radiusY);

            if(tunnel.branchesLeft > 0 && step > 25 && branchChanceDist(gen) < 4 && tunnels.size() < 140) {
                float branchSide = (branchChanceDist(gen) < 50) ? -1.0f : 1.0f;
                tunnels.push_back({
                    tunnel.x,
                    tunnel.y,
                    tunnel.angle + branchSide * (0.75f + std::abs(curve) * 0.9f),
                    std::max(1.4f, radius * 0.78f),
                    std::max(45, tunnel.length / 2),
                    tunnel.branchesLeft - 1
                });
            }

            tunnel.angle += curve * 0.22f;
            tunnel.x += std::cos(tunnel.angle) * 1.35f;
            tunnel.y += std::sin(tunnel.angle) * 0.75f;
            tunnel.y = std::clamp(tunnel.y, 122.0f, static_cast<float>(TOTAL_HEIGHT - 12));

            if(tunnel.x < 2.0f || tunnel.x >= TOTAL_WIDTH - 2.0f)
                break;
        }
    }

    for(int x = 0; x < TOTAL_WIDTH; x++) {
        for(int y = 0; y < TOTAL_HEIGHT; y++) {
            int block = getBlock(x, y);
            if(block != 3 && block != 4) {
                continue;
            }
            float coal = coalOre.GetNoise(static_cast<float>(x), static_cast<float>(y));

            if(coal > 0.80f) {
                if(block == 3) {
                    setBlock(x, y, 23);
                } else if(block == 4) {
                    float coal2 = coalOre.GetNoise(static_cast<float>(x), static_cast<float>(y));
                    if(coal > 0.95) {
                        setBlock(x, y, 24);
                    }
                }
            }
        }
        
    } 

    for(int x = 0; x < TOTAL_WIDTH; x++) {
        for(int y = 0; y < TOTAL_HEIGHT; y++) {
            int block = getBlock(x, y);
            if(block != 3 && block != 4) {
                continue;
            }
            float iron = ironOre.GetNoise(static_cast<float>(x), static_cast<float>(y));

            if(iron > 0.80f) {
                if(block == 3) {
                    setBlock(x, y, 27);
                } else if(block == 4) {
                    setBlock(x, y, 28);
                }
            }
        }
        
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
