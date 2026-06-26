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
    sf::Sprite woodSprite;
    sf::Sprite leafSprite;
    sf::Sprite planksSprite;
    sf::Sprite woodenPickaxeSprite;
    sf::Sprite stickSprite;
    sf::Sprite craftingTableSprite;
    sf::Sprite woodenAxeSprite;
    sf::Sprite woodenShovelSprite;
    sf::Sprite woodenSwordSprite;
    sf::Sprite stonePickaxeSprite;
    sf::Sprite stoneAxeSprite;
    sf::Sprite stoneShovelSprite;
    sf::Sprite stoneSwordSprite;
    sf::Sprite woodenSlabSprite;
    sf::Sprite cobblestoneSprite;
    sf::Sprite coalSprite;
    sf::Sprite coalOreStoneSprite;
    sf::Sprite coalOreDeepslateSprite;
    sf::Sprite ironIngotSprite;
    sf::Sprite rawIronSprite;
    sf::Sprite ironOreStoneSprite;
    sf::Sprite ironOreDeepslateSprite;
    sf::Sprite ovenSprite;
    sf::Sprite ironPickaxeSprite;
    sf::Sprite ironAxeSprite;
    sf::Sprite ironShovelSprite;
    sf::Sprite ironSwordSprite;
    sf::Sprite diamondSprite;
    sf::Sprite diamondOreStoneSprite;
    sf::Sprite diamondOreDeepslateSprite;

    sf::Sprite* getSprite(int itemID);

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
             sf::Texture& animationBreaking, 
             sf::Texture& wood,
             sf::Texture& leaf,
             sf::Texture& planks,
             sf::Texture& woodenPickaxe,
             sf::Texture& stick,
             sf::Texture& craftingTable,
             sf::Texture& woodenAxe,
             sf::Texture& woodenShovel,
             sf::Texture& woodenSword,
             sf::Texture& stonePickaxe,
             sf::Texture& stoneAxe,
             sf::Texture& stoneShovel,
             sf::Texture& stoneSword,
             sf::Texture& woodenSlab,
             sf::Texture& cobblestone,
             sf::Texture& coal,
             sf::Texture& coalOreStone,
             sf::Texture& coalOreDeepslate,
             sf::Texture& ironIngot,
             sf::Texture& rawIron,
             sf::Texture& ironOreStone,
             sf::Texture& ironOreDeepslate,
             sf::Texture& oven,
             sf::Texture& ironPickaxe,
             sf::Texture& ironAxe,
             sf::Texture& ironShovel,
             sf::Texture& ironSword, 
             sf::Texture& diamond,
             sf::Texture& diamondOreStone,
             sf::Texture& diamondOreDeepslate);

    void drawWorld(sf::RenderWindow& window, const World& world, int chunkMinX, int chunkMaxX, int chunkMinY, int chunkMaxY, int blockX, int blockY, Player& player);
    void drawPlayer(sf::RenderWindow& window, const Player& player);
    void drawHotbar(sf::RenderWindow& window, Inventory& inventory, const sf::Font& font, int selectedSlot);
    void drawItemEntity(sf::RenderWindow& window, const ItemEntity& item, int itemID);
    void drawItemOnMouse(sf::RenderWindow& window, const ItemStack& item, const sf::Font& font);   
    void drawInventory(sf::RenderWindow& window, Inventory& inventory, const sf::Font& font, bool isChest);
    void drawAnimatedBreaking(sf::RenderWindow& window, int breakingX, int breakingY, int currentFrame);
    void changeTexturePlayer(sf::Texture& newTexture, Player& player);
    void drawItemPlayer(Player& player, ItemStack& item, sf::RenderWindow& window);
    void drawCraftingGrid2x2(Inventory& craftingGrid, sf::RenderWindow& window, const sf::Font& font);
    void drawOutputSlot2x2(Inventory& output, sf::RenderWindow& window, const sf::Font& font);
    void drawCraftingGrid3x3(Inventory& craftingGrid, sf::RenderWindow& window, const sf::Font& font);
    void drawOutputSlot3x3(Inventory& output, sf::RenderWindow& window, const sf::Font& font);
    void drawOven(sf::RenderWindow& window, Inventory& ovenInventory, const sf::Font& font, float progress, float maxProgress, float fuelTime, float maxFuelTime);
};
