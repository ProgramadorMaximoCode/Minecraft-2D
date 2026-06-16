#include "renderer.h"

Renderer::Renderer(float tileSize, sf::Texture& dirt, sf::Texture& grass, sf::Texture& stone, sf::Texture& deepSlate, sf::Texture& bedrock, sf::Texture& chest, sf::Texture& animationBreaking, sf::Texture& wood, sf::Texture& leaf, sf::Texture& planks, sf::Texture& woodenPickaxe, sf::Texture& stick, sf::Texture& craftingTable, sf::Texture& woodenAxe, sf::Texture& woodenShovel, sf::Texture& woodenSword, sf::Texture& stonePickaxe, sf::Texture& stoneAxe, sf::Texture& stoneShovel, sf::Texture& stoneSword, sf::Texture& woodenSlab, sf::Texture& cobblestone)
    : tileSize(tileSize),
      dirtSprite(dirt),
      grassSprite(grass),
      stoneSprite(stone),
      deepSlateSprite(deepSlate),
      bedrockSprite(bedrock),
      chestSprite(chest),
      woodSprite(wood),
      leafSprite(leaf),
      planksSprite(planks),
      animationBreakingSprite(animationBreaking),
      woodenPickaxeSprite(woodenPickaxe),
      stickSprite(stick),
      craftingTableSprite(craftingTable),
      woodenAxeSprite(woodenAxe),
      woodenShovelSprite(woodenShovel),
      woodenSwordSprite(woodenSword),
      stonePickaxeSprite(stonePickaxe),
      stoneAxeSprite(stoneAxe),
      stoneShovelSprite(stoneShovel),
      stoneSwordSprite(stoneSword),
      woodenSlabSprite(woodenSlab),
      cobblestoneSprite(cobblestone)
      {
        dirtSprite.setScale({0.25f, 0.25f});
        grassSprite.setScale({0.25f, 0.25f});
        stoneSprite.setScale({0.25f, 0.25f});
        deepSlateSprite.setScale({0.25f, 0.25f});
        bedrockSprite.setScale({0.25f, 0.25f});
        chestSprite.setScale({0.25f, 0.25f});
        woodSprite.setScale({0.25f, 0.25f});
        leafSprite.setScale({0.25f, 0.25f});
        planksSprite.setScale({0.25f, 0.25f});
        animationBreakingSprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
        animationBreakingSprite.setScale({0.25f, 0.25f});
        woodenPickaxeSprite.setScale({0.25f, 0.25f});
        stickSprite.setScale({0.25f, 0.25f});
        craftingTableSprite.setScale({0.25f, 0.25f});
        woodenAxeSprite.setScale({0.25f, 0.25f});
        woodenShovelSprite.setScale({0.25f, 0.25f});
        woodenSwordSprite.setScale({0.25f, 0.25f});
        stonePickaxeSprite.setScale({0.25f, 0.25f});
        stoneAxeSprite.setScale({0.25f, 0.25f});
        stoneShovelSprite.setScale({0.25f, 0.25f});
        stoneSwordSprite.setScale({0.25f, 0.25f});
        woodenSlabSprite.setScale({0.25f, 0.25f});
        cobblestoneSprite.setScale({0.25f, 0.25f});
}

void Renderer::drawWorld(sf::RenderWindow& window, const World& world, int chunkMinX, int chunkMaxX, int chunkMinY, int chunkMaxY, int blockX, int blockY, Player& player)
{
    int playerBlockX = static_cast<int>(player.getPosition().x / tileSize);
    int playerBlockY = static_cast<int>(player.getPosition().y / tileSize);

    int dx = std::abs(blockX - playerBlockX);
    int dy = std::abs(blockY - playerBlockY);

    for(int i = 1; i <= 21; ++i) {
        getSprite(i)->setOrigin({0.f, 0.f});
        getSprite(i)->setScale({0.25f, 0.25f});

    }
    for (int chunkX = chunkMinX - 2; chunkX <= chunkMaxX + 2; ++chunkX)
    {
        for (int chunkY = chunkMinY - 2; chunkY <= chunkMaxY + 2; ++chunkY)
        {
            // Evitar salirte del mundo
            if (chunkX < 0 || chunkY < 0 ||
                chunkX >= World::WORLD_WIDTH ||
                chunkY >= World::WORLD_HEIGHT)
                continue;

            // Recorrer bloques dentro del chunk
            for (int localX = 0; localX < Chunk::SIZE; ++localX)
            {
                for (int localY = 0; localY < Chunk::SIZE; ++localY)
                {
                    int globalX = chunkX * Chunk::SIZE + localX;
                    int globalY = chunkY * Chunk::SIZE + localY;

                    int block = world.getBlock(globalX, globalY);

                    if(blockX == globalX && blockY == globalY) {
                        if(dx <= 5 && dy <= 5) {
                            sf::RectangleShape highlight(sf::Vector2f(tileSize - 2, tileSize - 2));
                            highlight.setPosition({globalX * tileSize, globalY * tileSize});
                            highlight.setOutlineColor(sf::Color::Black);
                            highlight.setOutlineThickness(2.f);
                            highlight.setFillColor(sf::Color::Transparent);
                            window.draw(highlight);
                        }
                        

                    }

                    
                    for(int i = 1; i <= 21; ++i) {
                        if(block == i) {
                            sf::Sprite* sprite = getSprite(i);
                            if(sprite) {
                                sprite->setPosition({globalX * tileSize, globalY * tileSize});
                                window.draw(*sprite);
                            }
                        }

                    }
                }
            }
        }
    }
}

void Renderer::drawPlayer(sf::RenderWindow& window, const Player& player) {
    window.draw(player.getSprite());
}

void Renderer::drawHotbar(sf::RenderWindow& window, Inventory& inventory, const sf::Font& font, int selectedSlot) {
     for (int i = 0; i < inventory.size(); i++) {
            sf::RectangleShape slot(sf::Vector2f(40.f, 40.f));
            slot.setPosition({
                static_cast<float>(200 + (i % 9) * 45),
                static_cast<float>(window.getSize().y - 60 + (i / 9) * 45)
            });
            if(i == selectedSlot) {
                slot.setFillColor(sf::Color(80, 80, 80, 220));
                slot.setOutlineColor(sf::Color(255, 255, 255));
                slot.setOutlineThickness(4.f);
            } else {
                slot.setFillColor(sf::Color(80, 80, 80, 220));
                slot.setOutlineColor(sf::Color(180, 180, 180));
                slot.setOutlineThickness(2.f);
            }

            window.draw(slot);

            ItemStack& item = inventory.getSlot(i);

            if (!item.isEmpty()) {
                const sf::Vector2f slotPosition = slot.getPosition();
                const sf::Vector2f slotSize = slot.getSize();

                const sf::Vector2f slotCenter(
                    slotPosition.x + slotSize.x / 2.f,
                    slotPosition.y + slotSize.y / 2.f
                );

                sf::Sprite* itemSprite = getSprite(item.itemID);
                if (itemSprite) {
                    itemSprite->setOrigin({itemSprite->getLocalBounds().position.x + itemSprite->getLocalBounds().size.x / 2.f, itemSprite->getLocalBounds().position.y + itemSprite->getLocalBounds().size.y / 2.f});
                    itemSprite->setPosition(slotCenter);
                    itemSprite->setScale({0.20f, 0.20f});
                    window.draw(*itemSprite);
                }

                const sf::Vector2f textBottomRight(
                    slotPosition.x + slotSize.x - 4.f,
                    slotPosition.y + slotSize.y - 3.f
                );

                sf::Text text(font, std::to_string(item.count), 20);
                text.setFillColor(sf::Color::White);
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin({
                    textBounds.position.x + textBounds.size.x,
                    textBounds.position.y + textBounds.size.y
                });
                text.setPosition(textBottomRight);
                if(item.count >= 2) {
                    window.draw(text);
                }
            }
        }
}

void Renderer::drawItemEntity(sf::RenderWindow& window, const ItemEntity& item, int itemID) {
    sf::Sprite* itemSprite = getSprite(item.itemID);
    if (itemSprite) {
        itemSprite->setOrigin({0.f, 0.f});
        itemSprite->setPosition(item.position);
        itemSprite->setScale({0.12f, 0.12f});
        window.draw(*itemSprite);
    }
}

void Renderer::drawItemOnMouse(sf::RenderWindow& window, const ItemStack& item, const sf::Font& font) {
    if (!item.isEmpty()) {
        const sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        const sf::Vector2f worldPosition = window.mapPixelToCoords(static_cast<sf::Vector2i>(mousePosition));

        sf::Sprite* itemSprite = getSprite(item.itemID);
        if (itemSprite) {
            itemSprite->setOrigin({0.f, 0.f});
            itemSprite->setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            itemSprite->setScale({0.15f, 0.15f});
            window.draw(*itemSprite);
        }

        sf::Text text(font, std::to_string(item.count), 12);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({
            textBounds.position.x + textBounds.size.x,
            textBounds.position.y + textBounds.size.y
        });
        text.setPosition(mousePosition + sf::Vector2f(16.f, 12.f));
        if(item.count >= 2) {
            window.draw(text);
        }
    }
}

void Renderer::drawInventory(sf::RenderWindow& window, Inventory& inventory, const sf::Font& font, bool isChest) {
    constexpr int columns = 9;
    constexpr float slotSize = 40.f;
    constexpr float slotSpacing = 5.f;
    constexpr float slotStep = slotSize + slotSpacing;
    constexpr float distanceFromHotbar = 12.f;

    const int rows = (inventory.size() + columns - 1) / columns;
    const float totalWidth = columns * slotSize + (columns - 1) * slotSpacing;
    const float totalHeight = rows * slotSize + (rows - 1) * slotSpacing;
    const float startX = (static_cast<float>(window.getSize().x) - totalWidth) / 2.f;
    const float hotbarY = static_cast<float>(window.getSize().y) - 60.f;
    const float startY = isChest ? hotbarY - distanceFromHotbar * 2 - totalHeight * 2 : hotbarY - totalHeight - distanceFromHotbar;

    sf::RectangleShape slot(sf::Vector2f(slotSize, slotSize));
    sf::RectangleShape background(sf::Vector2f(totalWidth + 80.f, totalHeight + 260.f));
    background.setPosition({startX - 40.f, startY - 200.f});
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setOutlineColor(sf::Color(200, 200, 200));
    background.setOutlineThickness(2.f);
    if(!isChest) {
        window.draw(background);
    }
    for (int i = 0; i < inventory.size(); i++) {
        slot.setPosition({
            startX + (i % columns) * slotStep,
            startY + (i / columns) * slotStep
        });
        slot.setFillColor(sf::Color(80, 80, 80, 220));
        slot.setOutlineColor(sf::Color(180, 180, 180));
        slot.setOutlineThickness(2.f);

        window.draw(slot);
        

        ItemStack& item = inventory.getSlot(i);

        if (!item.isEmpty()) {
            const sf::Vector2f slotPosition = slot.getPosition();
            const sf::Vector2f slotSize = slot.getSize();

            const sf::Vector2f slotCenter(
                slotPosition.x + slotSize.x / 2.f,
                slotPosition.y + slotSize.y / 2.f
            );

            int itemID = item.itemID;
                sf::Sprite* itemSprite = getSprite(itemID);
                if (itemSprite) {
                    itemSprite->setOrigin({itemSprite->getLocalBounds().position.x + itemSprite->getLocalBounds().size.x / 2.f, itemSprite->getLocalBounds().position.y + itemSprite->getLocalBounds().size.y / 2.f});
                    itemSprite->setPosition(slotCenter);
                    itemSprite->setScale({0.20f, 0.20f});
                    window.draw(*itemSprite);
                }

            const sf::Vector2f textBottomRight(
                slotPosition.x + slotSize.x - 4.f,
                slotPosition.y + slotSize.y - 3.f
            );

            sf::Text text(font, std::to_string(item.count), 20);
            text.setFillColor(sf::Color::White);
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin({
                textBounds.position.x + textBounds.size.x,
                textBounds.position.y + textBounds.size.y
            });
            text.setPosition(textBottomRight);
            if(item.count >= 2) {
                window.draw(text);
            }
        }
    }
}

void Renderer::drawAnimatedBreaking(sf::RenderWindow& window, int breakingX, int breakingY, int currentFrame) {
    if(breakingX == -1 || breakingY == -1) {
        return;
    }

    if(currentFrame < 0) {
        currentFrame = 0;
    }
    if(currentFrame >= totalFrames) {
        currentFrame = totalFrames - 1;
    }

    animationBreakingSprite.setPosition({breakingX * tileSize, breakingY * tileSize});
    animationBreakingSprite.setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
    window.draw(animationBreakingSprite);
}

void Renderer::changeTexturePlayer(sf::Texture& newTexture, Player& player) {
    player.setTexture(newTexture);
}

void Renderer::drawItemPlayer(Player& player, ItemStack& item, sf::RenderWindow& window) {

    const sf::Vector2f playerPosition = player.getPosition();
    const sf::Vector2f itemPosition(playerPosition.x + 15.f, playerPosition.y + 40.f);

    int itemID = item.itemID;
    sf::Sprite* itemSprite = getSprite(itemID);
    if (itemSprite) {
        itemSprite->setOrigin({0.f, 0.f});
        itemSprite->setPosition(itemPosition);
        itemSprite->setScale({0.10f, 0.10f});
        window.draw(*itemSprite);
    }

}

void Renderer::drawCraftingGrid2x2(Inventory& craftingGrid, sf::RenderWindow& window, const sf::Font& font) {
    constexpr float slotSize = 40.f;
    const float startX = window.getSize().x / 2.f  + 50.f;
    const float startY = window.getSize().y / 2.f + 10.f;

    sf::RectangleShape slot(sf::Vector2f(slotSize, slotSize));
    slot.setFillColor(sf::Color(80, 80, 80, 220));
    slot.setOutlineColor(sf::Color(180, 180, 180));
    slot.setOutlineThickness(2.f);
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            slot.setPosition({startX + j * (slotSize + 5.f), startY + i * (slotSize + 5.f)});
            window.draw(slot);

            ItemStack& item = craftingGrid.getSlot(i * 2 + j);

            if (!item.isEmpty()) {
                const sf::Vector2f slotPosition = slot.getPosition();
                const sf::Vector2f slotSize = slot.getSize();

                const sf::Vector2f slotCenter(
                    slotPosition.x + slotSize.x / 9.f,
                    slotPosition.y + slotSize.y / 9.f
                );

                int itemID = item.itemID;
                sf::Sprite* itemSprite = getSprite(itemID);
                if (itemSprite) {
                    itemSprite->setOrigin({0.f, 0.f});
                    itemSprite->setPosition(slotCenter);
                    itemSprite->setScale({0.20f, 0.20f});
                    window.draw(*itemSprite);
                }

                const sf::Vector2f textBottomRight(
                    slotPosition.x + slotSize.x - 4.f,
                    slotPosition.y + slotSize.y - 3.f
                );

                sf::Text text(font, std::to_string(item.count), 20);
                text.setFillColor(sf::Color::White);
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin({
                    textBounds.position.x + textBounds.size.x,
                    textBounds.position.y + textBounds.size.y
                });
                text.setPosition(textBottomRight);
                if(item.count >= 2) {
                    window.draw(text);
                }
            }
        }
    }
}

void Renderer::drawOutputSlot2x2(Inventory& output, sf::RenderWindow& window, const sf::Font& font) {
    constexpr float slotSize = 40.f;
    const float startX = window.getSize().x / 2.f + 55.f;
    const float startY = window.getSize().y / 2.f + 10.f;

    sf::RectangleShape slot(sf::Vector2f(slotSize, slotSize));
    slot.setFillColor(sf::Color(80, 80, 80, 220));
    slot.setOutlineColor(sf::Color(180, 180, 180));
    slot.setOutlineThickness(2.f);
    slot.setPosition({startX + 2 * (slotSize + 15.f), startY + slotSize / 2.f});
    window.draw(slot);

    ItemStack& item = output.getSlot(0);

    if (!item.isEmpty()) {
        const sf::Vector2f slotPosition = slot.getPosition();
        const sf::Vector2f slotSize = slot.getSize();

        const sf::Vector2f slotCenter(
            slotPosition.x + slotSize.x / 9.f,
            slotPosition.y + slotSize.y / 9.f
        );
        int itemID = item.itemID;
        sf::Sprite* itemSprite = getSprite(itemID);
        if (itemSprite) {
            itemSprite->setOrigin({0.f, 0.f});
            itemSprite->setPosition(slotCenter);
            itemSprite->setScale({0.20f, 0.20f});
            window.draw(*itemSprite);
        }
           
        const sf::Vector2f textBottomRight(
            slotPosition.x + slotSize.x - 4.f,
            slotPosition.y + slotSize.y - 3.f
        );

        sf::Text text(font, std::to_string(item.count), 20);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({
            textBounds.position.x + textBounds.size.x,
            textBounds.position.y + textBounds.size.y
        });
        text.setPosition(textBottomRight);
        if(item.count >= 2) {
            window.draw(text);
        }
    }
}

void Renderer::drawCraftingGrid3x3(Inventory& craftingGrid, sf::RenderWindow& window, const sf::Font& font) {
    constexpr float slotSize = 40.f;
    const float startX = window.getSize().x / 2.f + 10.f;
    const float startY = window.getSize().y / 2.f - 10.f;

    sf::RectangleShape slot(sf::Vector2f(slotSize, slotSize));
    slot.setFillColor(sf::Color(80, 80, 80, 220));
    slot.setOutlineColor(sf::Color(180, 180, 180));
    slot.setOutlineThickness(2.f);
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            slot.setPosition({startX + j * (slotSize + 5.f), startY + i * (slotSize + 5.f)});
            window.draw(slot);

            ItemStack& item = craftingGrid.getSlot(i * 3 + j);

            if (!item.isEmpty()) {
                const sf::Vector2f slotPosition = slot.getPosition();
                const sf::Vector2f slotSize = slot.getSize();

                const sf::Vector2f slotCenter(
                    slotPosition.x + slotSize.x / 9.f,
                    slotPosition.y + slotSize.y / 9.f
                );

                int itemID = item.itemID;
                sf::Sprite* itemSprite = getSprite(itemID);

                if (itemSprite) {
                    itemSprite->setOrigin({0.f, 0.f});
                    itemSprite->setPosition(slotCenter);
                    itemSprite->setScale({0.20f, 0.20f});
                    window.draw(*itemSprite);
                }

                const sf::Vector2f textBottomRight(
                    slotPosition.x + slotSize.x - 4.f,
                    slotPosition.y + slotSize.y - 3.f
                );

                sf::Text text(font, std::to_string(item.count), 20);
                text.setFillColor(sf::Color::White);
                sf::FloatRect textBounds = text.getLocalBounds();
                text.setOrigin({
                    textBounds.position.x + textBounds.size.x,
                    textBounds.position.y + textBounds.size.y
                });
                text.setPosition(textBottomRight);
                if(item.count >= 2) {
                    window.draw(text);
                }
            }
        }
    }
}

void Renderer::drawOutputSlot3x3(Inventory& output, sf::RenderWindow& window, const sf::Font& font) {
    constexpr float slotSize = 40.f;
    const float startX = window.getSize().x / 2.f + 20.f;
    const float startY = window.getSize().y / 2.f + 15.f;

    sf::RectangleShape slot(sf::Vector2f(slotSize, slotSize));
    slot.setFillColor(sf::Color(80, 80, 80, 220));
    slot.setOutlineColor(sf::Color(180, 180, 180));
    slot.setOutlineThickness(2.f);
    slot.setPosition({startX + 3 * (slotSize + 15.f), startY + slotSize / 2.f});
    window.draw(slot);

    ItemStack& item = output.getSlot(0);

    if (!item.isEmpty()) {
        const sf::Vector2f slotPosition = slot.getPosition();
        const sf::Vector2f slotSize = slot.getSize();

        const sf::Vector2f slotCenter(
            slotPosition.x + slotSize.x / 9.f,
            slotPosition.y + slotSize.y / 9.f
        );

        int itemID = item.itemID;
        sf::Sprite* itemSprite = getSprite(itemID);
        if (itemSprite) {
            itemSprite->setOrigin({0.f, 0.f});
            itemSprite->setPosition(slotCenter);
            itemSprite->setScale({0.20f, 0.20f});
            window.draw(*itemSprite);
        }

        const sf::Vector2f textBottomRight(
            slotPosition.x + slotSize.x - 4.f,
            slotPosition.y + slotSize.y - 3.f
        );

        sf::Text text(font, std::to_string(item.count), 20);
        text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({
            textBounds.position.x + textBounds.size.x,
            textBounds.position.y + textBounds.size.y
        });
        text.setPosition(textBottomRight);
        if(item.count >= 2) {
            window.draw(text);
        }
    }
}

sf::Sprite* Renderer::getSprite(int itemID) {
    switch(itemID) {
        case 1:  return &grassSprite;
        case 2:  return &dirtSprite;
        case 3:  return &stoneSprite;
        case 4:  return &deepSlateSprite;
        case 5:  return &bedrockSprite;
        case 6:  return &chestSprite;
        case 7:  return &woodSprite;
        case 8:  return &leafSprite;
        case 9:  return &planksSprite;
        case 10: return &woodenPickaxeSprite;
        case 11: return &stickSprite;
        case 12: return &craftingTableSprite;
        case 13: return &woodenAxeSprite;
        case 14: return &woodenShovelSprite;
        case 15: return &woodenSwordSprite;
        case 16: return &stonePickaxeSprite;
        case 17: return &stoneAxeSprite;
        case 18: return &stoneShovelSprite;
        case 19: return &stoneSwordSprite;
        case 20: return &woodenSlabSprite;
        case 21: return &cobblestoneSprite;
        default: return nullptr;
    }
}