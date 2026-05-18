#pragma once
#include <SFML/Graphics.hpp>
#include "world.h"
#include "player.h"
#include "inventory.h"
#include "ItemEntity.h"

class Renderer {
private:
    float tileSize;

    sf::Sprite dirtSprite;
    sf::Sprite grassSprite;
    sf::Sprite stoneSprite;
    sf::Sprite deepSlateSprite;
    sf::Sprite bedrockSprite;
    sf::Sprite chestSprite;
    sf::Sprite animationBreakingSprite;

    int frameWidth = 160;
    int frameHeight = 160;
    int totalFrames = 9;

public:
    Renderer(float tileSize,
             sf::Texture& dirt,
             sf::Texture& grass,
             sf::Texture& stone,
             sf::Texture& deepSlate,
             sf::Texture& bedrock,
             sf::Texture& chest,
             sf::Texture& animationBreaking);

    void drawWorld(sf::RenderWindow& window, const World& world, int chunkMinX, int chunkMaxX, int chunkMinY, int chunkMaxY, int blockX, int blockY, Player& player);
    void drawPlayer(sf::RenderWindow& window, const Player& player);
    void drawHotbar(sf::RenderWindow& window, Inventory& inventory, const sf::Font& font, int selectedSlot);
    void drawItemEntity(sf::RenderWindow& window, const ItemEntity& item, int itemID);
    void drawItemOnMouse(sf::RenderWindow& window, const ItemStack& item, const sf::Font& font);   
    void drawInventory(sf::RenderWindow& window, Inventory& inventory, const sf::Font& font, bool isChest);
    void drawAnimatedBreaking(sf::RenderWindow& window, int breakingX, int breakingY, int currentFrame);
    void changeTexturePlayer(sf::Texture& newTexture, Player& player);
    void drawItemPlayer(Player& player, ItemStack& item, sf::RenderWindow& window);
};
