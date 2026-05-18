#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "renderer.h"
#include "player.h"
#include "inventory.h"
#include "ItemEntity.h"
#include <vector>
#include <map>
#include <random>
#include <cmath>
#include <algorithm>


using namespace sf;

int main() {
    /*Music music;
    if (!music.openFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Sounds\\AriaMath.mp3")) {
        std::cerr << "No se pudo cargar Sounds\\AriaMath.mp3\n";
        return 1;
    }
    music.setVolume(200.f);
    music.setLooping(true);
    music.play();
    */
    RenderWindow window(VideoMode({800, 720}), "Minecraft 2D!");
    Font font;
    if (!font.openFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Fonts\\minecraft\\minecraft_font.ttf")) {
        std::cerr << "No se pudo cargar Fonts\\minecraft\\minecraft_font.ttf\n";
        return 1;
    }
    View view({400.f, 320.f}, {800.f, 720.f});
    Vector2f cameraPosition;
    Vector2i mousePosition = Mouse::getPosition();
    float smoothed = 1.f;
    World world;
    Clock clock;

    // ********
    // TEXTURAS
    //********* 

    Texture dirt, grass, stone, deepslate, bedrock, chest, steve, steve2,animationBreaking;

    if (!dirt.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Dirt.png")) return 1;
    if (!grass.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Grass.png")) return 1;
    if (!stone.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Stone.png")) return 1;
    if (!deepslate.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Deepslate.png")) return 1;
    if (!bedrock.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Bedrock.png")) return 1;
    if (!chest.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Chest.png")) return 1;
    if (!steve.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Steve.png")) return 1;
    if (!steve2.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Steve2.png")) return 1;
    if (!animationBreaking.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\AnimationBreaking.png")) return 1;

    int currentFrame = 0;
    int totalFrames = 9;

    Player player(steve, {0.f, 1000.f});
    std::vector<ItemEntity> items;

    float tileSize = dirt.getSize().x * 0.25f;
    float gravity = 500.f;

    Renderer renderer(tileSize, dirt, grass, stone, deepslate, bedrock, chest, animationBreaking);

    world.generate();
    world.setBlock(0, 70, 6);
    
    auto resolvePlayerCollisions = [&](Player& player, char axis) {
        sf::FloatRect hitbox = player.getHitbox();

        int minX = static_cast<int>(std::floor(hitbox.position.x / tileSize)) - 1;
        int maxX = static_cast<int>(std::floor((hitbox.position.x + hitbox.size.x) / tileSize)) + 1;
        int minY = static_cast<int>(std::floor(hitbox.position.y / tileSize)) - 1;
        int maxY = static_cast<int>(std::floor((hitbox.position.y + hitbox.size.y) / tileSize)) + 1;

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                if (!world.isSolid(world.getBlock(x, y)))
                    continue;

                sf::FloatRect blockBounds = world.getBlockHitbox(x, y, tileSize);

                if (!player.getHitbox().findIntersection(blockBounds))
                    continue;

                if (axis == 'x') {
                    if (player.getSpeedX() > 0.f) {
                        player.setPosition({
                            blockBounds.position.x - player.getHitbox().size.x - player.getHitboxOffset().x,
                            player.getPosition().y
                        });
                    } else if (player.getSpeedX() < 0.f) {
                        player.setPosition({
                            blockBounds.position.x + blockBounds.size.x - player.getHitboxOffset().x,
                            player.getPosition().y
                        });
                    }

                    player.setSpeedX(0.f);
                } else if (axis == 'y') {
                    if (player.getSpeedY() > 0.f) {
                        player.setPosition({
                            player.getPosition().x,
                            blockBounds.position.y - player.getHitbox().size.y - player.getHitboxOffset().y
                        });
                    } else if (player.getSpeedY() < 0.f) {
                        player.setPosition({
                            player.getPosition().x,
                            blockBounds.position.y + blockBounds.size.y - player.getHitboxOffset().y
                        });
                    }

                    player.setSpeedY(0.f);
                }
            }
        }
    };
    auto resolveItemCollisions = [&](ItemEntity& item, char axis) {
        sf::FloatRect hitbox = item.getHitbox();

        int minX = static_cast<int>(std::floor(hitbox.position.x / tileSize)) - 1;
        int maxX = static_cast<int>(std::floor((hitbox.position.x + hitbox.size.x) / tileSize)) + 1;
        int minY = static_cast<int>(std::floor(hitbox.position.y / tileSize)) - 1;
        int maxY = static_cast<int>(std::floor((hitbox.position.y + hitbox.size.y) / tileSize)) + 1;

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                if (!world.isSolid(world.getBlock(x, y)))
                    continue;

                sf::FloatRect blockBounds = world.getBlockHitbox(x, y, tileSize);

                if (!item.getHitbox().findIntersection(blockBounds))
                    continue;

                if (axis == 'x') {
                    if (item.velocity.x > 0.f) {
                        item.position.x = blockBounds.position.x - item.getHitbox().size.x;
                    } else if (item.velocity.x < 0.f) {
                        item.position.x = blockBounds.position.x + blockBounds.size.x;
                    }

                    item.velocity.x = 0.f;
                } else if (axis == 'y') {
                    if (item.velocity.y > 0.f) {
                        item.position.y = blockBounds.position.y - item.getHitbox().size.y;
                    } else if (item.velocity.y < 0.f) {
                        item.position.y = blockBounds.position.y + blockBounds.size.y;
                    }

                    item.velocity.y = 0.f;
                }
            }
        }
    };

    Inventory hotbar(9);
    Inventory inventory(27);
    ItemStack cursorItem;
    int selectedSlot = -1;
    bool hasCursorItem = false;
    bool isInventoryOpen = false;
    bool isChestOpen = false;

    std::vector<float> blockRequiredTimes = {
        0.75f, 0.75f, 7.5f, 15.0f, 9999999.9f, 3.75f
    };

    float breakTime = 0.0f;
    int breakingX = -1;
    int breakingY = -1;

    struct ClickedSlot {

        Inventory* inventory = nullptr;
        int index = -1;
    };

    struct Chest { 
        Inventory inventory; 
        Chest() : inventory(27) {} 
    };
    std::map<std::pair<int, int>, Chest> chests;
    Chest* openedChest = nullptr;

    world.setBlock(500, 100, 6);

    auto onLeftClick = [&](Inventory& inventory, int index) {
        constexpr int maxStackSize = 64;
        ItemStack& slot = inventory.getSlot(index);

        if (!hasCursorItem) {
            if (!slot.isEmpty()) {
                cursorItem = slot;
                slot = {};
                hasCursorItem = true;
            }
        } else {
            if (slot.isEmpty()) {
                slot = cursorItem;
                cursorItem = {};
                hasCursorItem = false;
            }
            else if (slot.itemID == cursorItem.itemID) {
                int spaceLeft = maxStackSize - slot.count;
                int amountToMove = std::min(cursorItem.count, spaceLeft);

                if (amountToMove > 0) {
                    slot.count += amountToMove;
                    cursorItem.count -= amountToMove;
                }

                if (cursorItem.count <= 0) {
                    cursorItem = {};
                    hasCursorItem = false;
                }
            }
            else {
                std::swap(slot, cursorItem);
            }
        }
    };

    auto onRightClick = [&](Inventory& inventory, int index) {
        ItemStack& slot = inventory.getSlot(index);

        if (!hasCursorItem) {
            if (!slot.isEmpty()) {
                int halfCount = (slot.count + 1) / 2;
                cursorItem = {slot.itemID, halfCount};
                slot.count -= halfCount;
                hasCursorItem = true;

                if (slot.count <= 0) {
                    slot = {};
                }
            }
        } else {
            if (slot.isEmpty()) {
                slot = {cursorItem.itemID, 1};
                cursorItem.count -= 1;
                if (cursorItem.count <= 0) {
                    cursorItem = {};
                    hasCursorItem = false;
                }
            }
            else if (slot.itemID == cursorItem.itemID) {
                for (int i = 0; i < cursorItem.count; i++) {
                    if (slot.count >= 64) {
                        break;
                    }
                    slot.count++;
                    cursorItem.count -= 1;
                }
                if (cursorItem.count <= 0) {
                    cursorItem = {};
                    hasCursorItem = false;
                }
            }

        }
    };

    auto getSlotIndex = [&](sf::Vector2i mousePos, sf::RenderWindow& window, Inventory& inventory, int nInventory) {
        constexpr int columns = 9;
        constexpr float slotSize = 40.f;
        constexpr float slotSpacing = 5.f;
        constexpr float slotStep = slotSize + slotSpacing;

        const int rows = (inventory.size() + columns - 1) / columns;
        const float totalWidth = columns * slotSize + (columns - 1) * slotSpacing;
        const float hotbarY = static_cast<float>(window.getSize().y) - 60.f;

        float startX, startY;

        if(nInventory == 0) {
            startX = 200.f;
            startY = hotbarY;
        } else if(nInventory == 1) {
            startX = (static_cast<float>(window.getSize().x) - totalWidth) / 2.f;
            startY = hotbarY - 12.f - (rows * slotSize + (rows - 1) * slotSpacing);
        } else if(nInventory == 2) {
            startX = (static_cast<float>(window.getSize().x) - totalWidth) / 2.f;
            startY = (hotbarY - 12.f - (rows * slotSize + (rows - 1) * slotSpacing) * 2);
        }

        for (int i = 0; i < inventory.size(); i++) {
            sf::FloatRect slotBounds(
                {startX + (i % columns) * slotStep, startY + (i / columns) * slotStep},
                {slotSize, slotSize}
            );

            if (slotBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                return i;
            }
        }

        return -1; 
    };

    auto getClickedSlot = [&](sf::Vector2i mousePos, sf::RenderWindow& window) {
        if (isInventoryOpen) {
            int inventoryIndex = getSlotIndex(mousePos, window, inventory, 1);
            if (inventoryIndex != -1) {
                return ClickedSlot{&inventory, inventoryIndex};
            }
        }

        int hotbarIndex = getSlotIndex(mousePos, window, hotbar, 0);
        if (hotbarIndex != -1) {
            return ClickedSlot{&hotbar, hotbarIndex};
        }

        if (isChestOpen && openedChest != nullptr) {
            int chestIndex = getSlotIndex(mousePos, window, openedChest->inventory, 2);
            if (chestIndex != -1) {
                return ClickedSlot{&openedChest->inventory, chestIndex};
            }
        }

        return ClickedSlot{};
    };

    auto dropChestContents = [&](int blockX, int blockY) {
        auto chestIt = chests.find({blockX, blockY});
        if(chestIt == chests.end()) {
            return;
        }

        for(int i = 0; i < chestIt->second.inventory.size(); ++i) {
            if(!chestIt->second.inventory.getSlot(i).isEmpty()) {
                items.push_back(ItemEntity{
                    chestIt->second.inventory.getSlot(i).itemID,
                    chestIt->second.inventory.getSlot(i).count,
                    {blockX * tileSize, blockY * tileSize},
                    {0.f, -100.f}
                });
                chestIt->second.inventory.getSlot(i) = {};
            }
        }

        if(openedChest == &chestIt->second) {
            openedChest = nullptr;
            isChestOpen = false;
        }

        chests.erase(chestIt);
    };

    auto breakBlock = [&](int blockX, int blockY, float dt) {
            int playerBlockX = static_cast<int>(player.getPosition().x / tileSize);
            int playerBlockY = static_cast<int>(player.getPosition().y / tileSize);

            int dx = std::abs(blockX - playerBlockX);
            int dy = std::abs(blockY - playerBlockY);

            if(dx <= 5 && dy <= 5 && world.getBlock(blockX, blockY) != 0) {
                if(breakingX != blockX || breakingY != blockY) {
                    breakingX = blockX;
                    breakingY = blockY;
                    breakTime = 0.0f;
                    currentFrame = 0;
                }

                breakTime += dt;
                currentFrame = std::min(totalFrames - 1, static_cast<int>((breakTime / blockRequiredTimes[world.getBlock(blockX, blockY) - 1]) * totalFrames));

                if(breakTime >= blockRequiredTimes[world.getBlock(blockX, blockY) - 1]) {
                    int block = world.getBlock(blockX, blockY);

                    if(block == 6) {
                        dropChestContents(blockX, blockY);
                    }

                    items.push_back(ItemEntity{
                        block,
                        1,
                        {blockX * tileSize, blockY * tileSize},
                        {0.f, -100.f}
                    });

                    world.setBlock(blockX, blockY, 0);

                    breakTime = 0.0f;
                    breakingX = -1;
                    breakingY = -1;
                    currentFrame = 0;
                }
            } else {
                breakTime = 0.0f;
                breakingX = -1;
                breakingY = -1;
                currentFrame = 0;
            }
    };

    while (window.isOpen()) {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, view);

        int blockX = static_cast<int>(mouseWorld.x / tileSize);
        int blockY = static_cast<int>(mouseWorld.y / tileSize); 
        float left = view.getCenter().x - view.getSize().x / 2;
        float right = view.getCenter().x + view.getSize().x / 2;
        float top = view.getCenter().y - view.getSize().y / 2;
        float bottom = view.getCenter().y + view.getSize().y / 2;

        int blockLeft = left / tileSize;
        int blockRight = right / tileSize;
        int blockTop = top / tileSize;
        int blockBottom = bottom / tileSize;

        int chunkMinX = blockLeft / World::CHUNK_SIZE;
        int chunkMaxX = blockRight / World::CHUNK_SIZE;

        int chunkMinY = blockTop / World::CHUNK_SIZE;
        int chunkMaxY = blockBottom / World::CHUNK_SIZE;

        float dt = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();
            if (event->is<Event::KeyPressed>()) {
                if(Keyboard::isKeyPressed(Keyboard::Key::E)) {
                    isInventoryOpen = !isInventoryOpen;
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    ClickedSlot clickedSlot = getClickedSlot(mousePixel, window);
                    if(clickedSlot.index != -1) {
                        onLeftClick(*clickedSlot.inventory, clickedSlot.index);
                    }
                    else if(world.getBlock(blockX, blockY) == 0 && isInventoryOpen) {
                        if(hasCursorItem) {
                            items.push_back(ItemEntity{cursorItem.itemID, cursorItem.count, {blockX * tileSize, blockY * tileSize}, {0.f, -100.f}});
                            cursorItem = {};
                            hasCursorItem = false;
                        }
                    }
                } 
                else if (mouseButton->button == Mouse::Button::Right) {
                    ClickedSlot clickedSlot = getClickedSlot(mousePixel, window);
                    if(clickedSlot.index != -1) {
                        onRightClick(*clickedSlot.inventory, clickedSlot.index);
                    } else if(world.getBlock(blockX, blockY) == 6) {
                        isChestOpen = !isChestOpen;
                        if(isChestOpen) {
                            openedChest = &chests[{blockX, blockY}];
                        } else {
                            openedChest = nullptr;
                        }
                        
                    }
                    else if(!player.getHitbox().findIntersection(world.getBlockHitbox(blockX, blockY, tileSize))) {
                        if(selectedSlot != -1) {
                            ItemStack& item = hotbar.getSlot(selectedSlot);

                            if(!item.isEmpty() && world.getBlock(blockX, blockY) == 0) {
                                world.setBlock(blockX, blockY, hotbar.getSlot(selectedSlot).itemID);
                                item.count--;
                                if(item.count <= 0) {
                                    item = {};
                                }
                            }
                        }
                    }
                }
            }
        }

        if(Mouse::isButtonPressed(Mouse::Button::Left) && !isInventoryOpen) {
            breakBlock(blockX, blockY, dt);
        } else {
            breakTime = 0.0f;
            breakingX = -1;
            breakingY = -1;
            currentFrame = 0;
        }

        player.setSpeedX(0.f);

        if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A)) {
            player.setSpeedX(-200.f);
            renderer.changeTexturePlayer(steve2, player);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D)) {
            player.setSpeedX(200.f);
            renderer.changeTexturePlayer(steve, player);
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W) ) {
            if(player.getSpeedY() == 0.f) {
                player.setSpeedY(-250.f);
            }
        }

        player.setSpeedY(player.getSpeedY() + gravity * dt);

        player.setPosition({
            player.getPosition().x + player.getSpeedX() * dt,
            player.getPosition().y
        });
        resolvePlayerCollisions(player, 'x');

        player.setPosition({
            player.getPosition().x,
            player.getPosition().y + player.getSpeedY() * dt
        });
        resolvePlayerCollisions(player, 'y');

        for (auto& item : items) {
            item.position.x += item.velocity.x * dt;
            resolveItemCollisions(item, 'x');

            item.position.y += item.velocity.y * dt;
            resolveItemCollisions(item, 'y');
        }
        if (!items.empty()) {
            for (int i = items.size() - 1; i >= 0; --i) {
                if (player.getHitbox().findIntersection(items[i].getHitbox())) {
                    bool canAddToHotbar = false;
                    for(int j = 0; j < hotbar.size(); ++j) {
                        if (hotbar.getSlot(j).isEmpty() || hotbar.getSlot(j).itemID == items[i].itemID) {
                            canAddToHotbar = true;
                            break;
                        }
                    }
                    if(canAddToHotbar) {
                        hotbar.addItem(items[i].itemID, items[i].count);
                        items[i] = items.back();
                        items.pop_back();
                        continue;
                    }
                    
                    bool canAddToInventory = false;
                    for (int j = 0; j < inventory.size(); ++j) {
                        if (inventory.getSlot(j).isEmpty() || inventory.getSlot(j).itemID == items[i].itemID) {
                            canAddToInventory = true;
                            break;
                        }
                    }
                    if(canAddToInventory) {
                        inventory.addItem(items[i].itemID, items[i].count);
                        items[i] = items.back();
                        items.pop_back();
                        continue;
                    }
                }
            }
        }
        if(!items.empty()) {
            for (auto& item : items) {
                item.update(dt);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) selectedSlot = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) selectedSlot = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) selectedSlot = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) selectedSlot = 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) selectedSlot = 4;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) selectedSlot = 5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) selectedSlot = 6;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) selectedSlot = 7;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) selectedSlot = 8;

        Vector2f target = player.getPosition();

        cameraPosition = view.getCenter() + (target - view.getCenter()) * smoothed;

        view.setCenter(cameraPosition);
        window.setView(view);

        window.clear(sf::Color(104, 178, 202));

        renderer.drawWorld(window, world, chunkMinX, chunkMaxX, chunkMinY, chunkMaxY, blockX, blockY, player);
        renderer.drawPlayer(window, player);
        for (auto& item : items) {
            renderer.drawItemEntity(window, item, item.itemID);
        }
        if(selectedSlot != -1) {
            ItemStack& item = hotbar.getSlot(selectedSlot);
            if(!item.isEmpty()) {
                renderer.drawItemPlayer(player, item, window);
            }

        }

        if(!items.empty()) {
            for (const auto& item : items) {
                sf::RectangleShape itemHitboxR(item.getHitbox().size);
                itemHitboxR.setPosition(item.getHitbox().position);
                itemHitboxR.setFillColor(sf::Color::Transparent);
                itemHitboxR.setOutlineColor(sf::Color::Yellow);
                itemHitboxR.setOutlineThickness(1.f);
                window.draw(itemHitboxR);
            }
        }

        sf::RectangleShape hitboxR(player.getHitbox().size);
        hitboxR.setPosition(player.getHitbox().position);
        hitboxR.setFillColor(sf::Color::Transparent);
        hitboxR.setOutlineColor(sf::Color::Red);
        hitboxR.setOutlineThickness(1.f);
        window.draw(hitboxR);

        if(breakingX != -1 && breakingY != -1) {
            renderer.drawAnimatedBreaking(window, breakingX, breakingY, currentFrame);
        }

        window.setView(window.getDefaultView());

        renderer.drawHotbar(window, hotbar, font, selectedSlot);
        if (isInventoryOpen && !isChestOpen) {
            renderer.drawInventory(window, inventory, font, false);
        }
        if (isChestOpen) {
            isInventoryOpen = true;
            renderer.drawInventory(window, inventory, font, false);

            if (openedChest != nullptr) {
                renderer.drawInventory(window, openedChest->inventory, font, true);
            }
        }
        
        if (!cursorItem.isEmpty()) {
            renderer.drawItemOnMouse(window, cursorItem, font);
        }
        

        window.display();
    }

    return 0;
}
