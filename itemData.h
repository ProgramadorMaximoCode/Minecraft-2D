#pragma once
#include <SFML/Graphics.hpp>
#include "ItemStack.h"

enum toolTypes{
    NONE,
    PICKAXE,
    SHOVEL,
    SWORD,
    AXE,
    HOE
};

enum ItemsName {
        AIR,
        GRASS,
        DIRT,
        STONE,
        DEEPSLATE,
        BEDROCK,
        CHEST,
        WOOD,
        LEAF,
        PLANKS,
        WOODEN_PICKAXE,
        STICK,
        CRAFTING_TABLE,
        WOODEN_AXE,
        WOODEN_SHOVEL,
        WOODEN_SWORD,
        STONE_PICKAXE,
        STONE_AXE,
        STONE_SHOVEL,
        STONE_SWORD,
        WOODEN_SLAB,
        COBBLESTONE,
        COAL, 
        COAL_ORE_STONE,
        COAL_ORE_DEEPSLATE,
        IRON_INGOT, 
        RAW_IRON, 
        IRON_ORE_STONE, 
        IRON_ORE_DEEPSLATE, 
        OVEN,
        IRON_PICKAXE,
        IRON_AXE,
        IRON_SHOVEL,
        IRON_SWORD,
        DIAMOND, 
        DIAMOND_ORE_STONE,
        DIAMOND_ORE_DEEPSLATE,
};

struct ItemData {
    int itemID;
    ItemsName name;
    const sf::Texture* texture;
    bool isStackable;
    int maxStackSize;
    bool isBlock;
    toolTypes toolType;
    int toolLevel;
    int toolSpeed;
    float hardness;
    toolTypes requiredTool;
    ItemStack output;
    int levelRequiredToDrop;

        ItemData(int id, ItemsName itemName, const sf::Texture* itemTexture, bool stackable, int maxStack, bool block, toolTypes tool, int level, int speed,float hard, toolTypes required, const ItemStack& out, int levelrequired)
                : itemID(id),
                    name(itemName),
                    texture(itemTexture),
                    isStackable(stackable),
                    maxStackSize(maxStack),
                    isBlock(block),
                    toolType(tool),
                    toolLevel(level),
                    toolSpeed(speed),
                    hardness(hard),
                    requiredTool(required),
                    output(out), 
                    levelRequiredToDrop(levelrequired) {}
};
