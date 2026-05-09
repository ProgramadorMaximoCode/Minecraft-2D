#include "renderer.h"

Renderer::Renderer(float tileSize, sf::Texture& dirt, sf::Texture& grass, sf::Texture& stone, sf::Texture& deepSlate, sf::Texture& bedrock)
    : tileSize(tileSize),
      dirtSprite(dirt),
      grassSprite(grass),
      stoneSprite(stone),
      deepSlateSprite(deepSlate),
      bedrockSprite(bedrock) {
        dirtSprite.setScale({0.25f, 0.25f});
        grassSprite.setScale({0.25f, 0.25f});
        stoneSprite.setScale({0.25f, 0.25f});
        deepSlateSprite.setScale({0.25f, 0.25f});
        bedrockSprite.setScale({0.25f, 0.25f});
}

void Renderer::drawWorld(sf::RenderWindow& window, const World& world, int chunkMinX, int chunkMaxX, int chunkMinY, int chunkMaxY)
{
    dirtSprite.setScale({0.25f, 0.25f});
    grassSprite.setScale({0.25f, 0.25f});
    stoneSprite.setScale({0.25f, 0.25f});
    deepSlateSprite.setScale({0.25f, 0.25f});
    bedrockSprite.setScale({0.25f, 0.25f});

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

                    Chunk::BlockType block = world.getBlock(globalX, globalY);

                    switch (block)
                    {
                        case Chunk::BlockType::Air:
                            break;

                        case Chunk::BlockType::Dirt:
                            dirtSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(dirtSprite);
                            break;

                        case Chunk::BlockType::Grass:
                            grassSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(grassSprite);
                            break;

                        case Chunk::BlockType::Stone:
                            stoneSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(stoneSprite);
                            break;

                        case Chunk::BlockType::DeepSlate:
                            deepSlateSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(deepSlateSprite);
                            break;

                        case Chunk::BlockType::Bedrock:
                            bedrockSprite.setPosition({globalX * tileSize, globalY * tileSize});
                            window.draw(bedrockSprite);
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

void Renderer::drawInventory(sf::RenderWindow& window, Inventory& inventory, int selectedSlot, const sf::Font& font) {
     for (int i = 0; i < inventory.size(); i++) {
            sf::RectangleShape slot(sf::Vector2f(40.f, 40.f));
            slot.setPosition({
                static_cast<float>(200 + (i % 9) * 45),
                static_cast<float>(window.getSize().y - 60 + (i / 9) * 45)
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

                dirtSprite.setPosition(slotCenter);
                dirtSprite.setScale({0.20f, 0.20f});

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

                window.draw(dirtSprite);
                window.draw(text);
            }
        }
}

void Renderer::drawItemEntity(sf::RenderWindow& window, const ItemEntity& item) {
    dirtSprite.setPosition(item.getPosition());
    dirtSprite.setScale({0.12f, 0.12f});
    window.draw(dirtSprite);
}
