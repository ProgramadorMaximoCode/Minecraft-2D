#include "renderer.h"

Renderer::Renderer(float tileSize, sf::Texture& dirt, sf::Texture& grass, sf::Texture& stone, sf::Texture& deepSlate, sf::Texture& bedrock, sf::Texture& chest, sf::Texture& animationBreaking)
    : tileSize(tileSize),
      dirtSprite(dirt),
      grassSprite(grass),
      stoneSprite(stone),
      deepSlateSprite(deepSlate),
      bedrockSprite(bedrock),
      chestSprite(chest),
      animationBreakingSprite(animationBreaking) {
        dirtSprite.setScale({0.25f, 0.25f});
        grassSprite.setScale({0.25f, 0.25f});
        stoneSprite.setScale({0.25f, 0.25f});
        deepSlateSprite.setScale({0.25f, 0.25f});
        bedrockSprite.setScale({0.25f, 0.25f});
        chestSprite.setScale({0.25f, 0.25f});
        animationBreakingSprite.setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
        animationBreakingSprite.setScale({0.25f, 0.25f});
}

void Renderer::drawWorld(sf::RenderWindow& window, const World& world, int chunkMinX, int chunkMaxX, int chunkMinY, int chunkMaxY, int blockX, int blockY, Player& player)
{
    int playerBlockX = static_cast<int>(player.getPosition().x / tileSize);
    int playerBlockY = static_cast<int>(player.getPosition().y / tileSize);

    int dx = std::abs(blockX - playerBlockX);
    int dy = std::abs(blockY - playerBlockY);

    dirtSprite.setScale({0.25f, 0.25f});
    grassSprite.setScale({0.25f, 0.25f});
    stoneSprite.setScale({0.25f, 0.25f});
    deepSlateSprite.setScale({0.25f, 0.25f});
    bedrockSprite.setScale({0.25f, 0.25f});
    chestSprite.setScale({0.25f, 0.25f});
    animationBreakingSprite.setScale({0.25f, 0.25f});

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
                            sf::RectangleShape highlight(sf::Vector2f(tileSize, tileSize));
                            highlight.setPosition({globalX * tileSize, globalY * tileSize});
                            highlight.setOutlineColor(sf::Color::Black);
                            highlight.setOutlineThickness(2.f);
                            highlight.setFillColor(sf::Color::Transparent);
                            window.draw(highlight);
                        }
                        

                    }

                    switch (block)
                    {
                        case 0:
                            break;

                        case 2:
                            dirtSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(dirtSprite);
                            break;

                        case 1:
                            grassSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(grassSprite);
                            break;

                        case 3:
                            stoneSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(stoneSprite);
                            break;

                        case 4:
                            deepSlateSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(deepSlateSprite);
                            break;

                        case 5:
                            bedrockSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(bedrockSprite);
                            break;

                        case 6:
                            chestSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(chestSprite);
                            break;
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
                    slotPosition.x + slotSize.x / 9.f,
                    slotPosition.y + slotSize.y / 9.f
                );

                int itemID = item.itemID;
                if (itemID == 1) {
                    grassSprite.setPosition(slotCenter);
                    grassSprite.setScale({0.20f, 0.20f});
                    window.draw(grassSprite);
                } else if (itemID == 2) {
                    dirtSprite.setPosition(slotCenter);
                    dirtSprite.setScale({0.20f, 0.20f});
                    window.draw(dirtSprite);
                } else if (itemID == 3) {
                    stoneSprite.setPosition(slotCenter);
                    stoneSprite.setScale({0.20f, 0.20f});
                    window.draw(stoneSprite);
                } else if (itemID == 4) {
                    deepSlateSprite.setPosition(slotCenter);
                    deepSlateSprite.setScale({0.20f, 0.20f});
                    window.draw(deepSlateSprite);
                } else if (itemID == 5) {
                    bedrockSprite.setPosition(slotCenter);
                    bedrockSprite.setScale({0.20f, 0.20f});
                    window.draw(bedrockSprite);
                } else if (itemID == 6) {
                    chestSprite.setPosition(slotCenter);
                    chestSprite.setScale({0.20f, 0.20f});
                    window.draw(chestSprite);
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
    if(itemID == 1) {
        grassSprite.setPosition(item.getPosition());
        grassSprite.setScale({0.12f, 0.12f});
        window.draw(grassSprite);
    } else if(itemID == 2) {
        dirtSprite.setPosition(item.getPosition());
        dirtSprite.setScale({0.12f, 0.12f});
        window.draw(dirtSprite);
    } else if(itemID == 3) {
        stoneSprite.setPosition(item.getPosition());
        stoneSprite.setScale({0.12f, 0.12f});
        window.draw(stoneSprite);
    }
    else if(itemID == 4) {
        deepSlateSprite.setPosition(item.getPosition());
        deepSlateSprite.setScale({0.12f, 0.12f});
        window.draw(deepSlateSprite);
    }
    else if(itemID == 5) {
        bedrockSprite.setPosition(item.getPosition());
        bedrockSprite.setScale({0.12f, 0.12f});
        window.draw(bedrockSprite);
    } else if (itemID == 6) {
        chestSprite.setPosition(item.getPosition());
        chestSprite.setScale({0.12f, 0.12f});
        window.draw(chestSprite);
    }
    
}

void Renderer::drawItemOnMouse(sf::RenderWindow& window, const ItemStack& item, const sf::Font& font) {
    if (!item.isEmpty()) {
        const sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        const sf::Vector2f worldPosition = window.mapPixelToCoords(static_cast<sf::Vector2i>(mousePosition));

        int itemID = item.itemID;
        if (itemID == 1) {
            grassSprite.setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            grassSprite.setScale({0.15f, 0.15f});
            window.draw(grassSprite);
        } else if (itemID == 2) {
            dirtSprite.setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            dirtSprite.setScale({0.15f, 0.15f});
            window.draw(dirtSprite);
        } else if (itemID == 3) {
            stoneSprite.setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            stoneSprite.setScale({0.15f, 0.15f});
            window.draw(stoneSprite);
        } else if (itemID == 4) {
            deepSlateSprite.setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            deepSlateSprite.setScale({0.15f, 0.15f});
            window.draw(deepSlateSprite);
        } else if (itemID == 5) {
            bedrockSprite.setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            bedrockSprite.setScale({0.15f, 0.15f});
            window.draw(bedrockSprite);
        } else if (itemID == 6) {
            chestSprite.setPosition({worldPosition.x - 12.f, worldPosition.y - 12.f});
            chestSprite.setScale({0.15f, 0.15f});
            window.draw(chestSprite);
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
                slotPosition.x + slotSize.x / 9.f,
                slotPosition.y + slotSize.y / 9.f
            );

            int itemID = item.itemID;
            if (itemID == 1) {
                grassSprite.setPosition(slotCenter);
                grassSprite.setScale({0.20f, 0.20f});
                window.draw(grassSprite);
            } else if (itemID == 2) {
                dirtSprite.setPosition(slotCenter);
                dirtSprite.setScale({0.20f, 0.20f});
                window.draw(dirtSprite);
            } else if (itemID == 3) {
                stoneSprite.setPosition(slotCenter);
                stoneSprite.setScale({0.20f, 0.20f});
                window.draw(stoneSprite);
            } else if (itemID == 4) {
                deepSlateSprite.setPosition(slotCenter);
                deepSlateSprite.setScale({0.20f, 0.20f});
                window.draw(deepSlateSprite);
            } else if (itemID == 5) {
                bedrockSprite.setPosition(slotCenter);
                bedrockSprite.setScale({0.20f, 0.20f});
                window.draw(bedrockSprite);
            } else if (itemID == 6) {
                chestSprite.setPosition(slotCenter);
                chestSprite.setScale({0.20f, 0.20f});
                window.draw(chestSprite);
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
    if (itemID == 1) {
        grassSprite.setPosition(itemPosition);
        grassSprite.setScale({0.10f, 0.10f});
        window.draw(grassSprite);
    } else if (itemID == 2) {
        dirtSprite.setPosition(itemPosition);
        dirtSprite.setScale({0.10f, 0.10f});
        window.draw(dirtSprite);
    } else if (itemID == 3) {
        stoneSprite.setPosition(itemPosition);
        stoneSprite.setScale({0.10f, 0.10f});
        window.draw(stoneSprite);
    } else if (itemID == 4) {
        deepSlateSprite.setPosition(itemPosition);
        deepSlateSprite.setScale({0.10f, 0.10f});
        window.draw(deepSlateSprite);
    } else if (itemID == 5) {
        bedrockSprite.setPosition(itemPosition);
        bedrockSprite.setScale({0.10f, 0.10f});
        window.draw(bedrockSprite);
    } else if (itemID == 6) {
        chestSprite.setPosition(itemPosition);
        chestSprite.setScale({0.10f, 0.10f});
        window.draw(chestSprite);
    }
    
}