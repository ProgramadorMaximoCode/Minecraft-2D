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

public:
    Renderer(float tileSize,
             sf::Texture& dirt,
             sf::Texture& grass,
             sf::Texture& stone,
             sf::Texture& deepSlate,
             sf::Texture& bedrock);

    void drawWorld(sf::RenderWindow& window, const World& world, int chunkMinX, int chunkMaxX, int chunkMinY, int chunkMaxY);
    void drawPlayer(sf::RenderWindow& window, const Player& player);
    void drawInventory(sf::RenderWindow& window, Inventory& inventory, int selectedSlot, const sf::Font& font);
    void drawItemEntity(sf::RenderWindow& window, const ItemEntity& item);
};
