#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "renderer.h"
#include "player.h"
#include "inventory.h"
#include "ItemEntity.h"
#include <vector>
#include <random>
#include <cmath>

using namespace sf;

int main() {
    Music music;
    if (!music.openFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Sounds\\AriaMath.mp3")) {
        std::cerr << "No se pudo cargar Sounds\\AriaMath.mp3\n";
        return 1;
    }
    music.setVolume(200.f);
    music.setLooping(true);
    music.play();

    RenderWindow window(VideoMode({800, 720}), "Minecraft 2D!");
    Font font("C:/Windows/Fonts/arial.ttf");
    View view({400.f, 320.f}, {800.f, 720.f});
    Vector2f cameraPosition;
    Vector2i mousePosition = Mouse::getPosition();
    float smoothed = 1.f;
    World world;
    Clock clock;

    Texture dirt, grass, stone, deepslate, bedrock, steve;

    if (!dirt.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Dirt.png")) return 1;
    if (!grass.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Grass.png")) return 1;
    if (!stone.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Stone.png")) return 1;
    if (!deepslate.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\DeepSlate.png")) return 1;
    if (!bedrock.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Bedrock.png")) return 1;
    if (!steve.loadFromFile("C:\\Programacion\\SFML\\Minecraft 2D\\Assets\\Steve.png")) return 1;

    Player player(steve, {10000.f, 1000.f});
    std::vector<ItemEntity> items;
    items.push_back(ItemEntity{1, 1, {10100.f, 1000.f}, {0.f, -100.f}});
    items.push_back(ItemEntity{1, 1, {10200.f, 1000.f}, {0.f, -100.f}});
    items.push_back(ItemEntity{1, 1, {10300.f, 1000.f}, {0.f, -100.f}});

    float tileSize = dirt.getSize().x * 0.25f;
    float gravity = 500.f;

    Renderer renderer(tileSize, dirt, grass, stone, deepslate, bedrock);

    world.generate();

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

    Inventory inventory(9);
    int selectedSlot = -1;

    auto onLeftClick = [&](int slotIndex) {
        if (selectedSlot == -1) {
            selectedSlot = slotIndex;
        } else {
            std::swap(inventory.getSlot(selectedSlot),
                    inventory.getSlot(slotIndex));
            selectedSlot = -1;
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

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    if(world.getBlock(blockX, blockY) != Chunk::BlockType::Air) {
                        world.setBlock(blockX, blockY, Chunk::BlockType::Air);
                        items.push_back(ItemEntity{1, 1, {blockX * tileSize, blockY * tileSize}, {0.f, -100.f}});
                    }
                } 
                else if (mouseButton->button == Mouse::Button::Right) {
                    if(!player.getHitbox().findIntersection(world.getBlockHitbox(blockX, blockY, tileSize))) {
                        world.setBlock(blockX, blockY, Chunk::BlockType::Dirt);
                    }

                }
            }
        }

        player.setSpeedX(0.f);

        if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
            player.setSpeedX(-200.f);
        if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D))
            player.setSpeedX(200.f);
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

        for (int i = items.size() - 1; i >= 0; --i) {
            if (player.getHitbox().findIntersection(items[i].getHitbox())) {
                inventory.addItem(items[i].itemID, items[i].count);
                items[i] = items.back();
                items.pop_back();
            }
        }

        for (auto& item : items) {
            item.update(dt);
        }

        Vector2f target = player.getPosition();

        cameraPosition = view.getCenter() + (target - view.getCenter()) * smoothed;

        view.setCenter(cameraPosition);
        window.setView(view);

        window.clear(sf::Color(104, 178, 202));

        renderer.drawWorld(window, world, chunkMinX, chunkMaxX, chunkMinY, chunkMaxY);
        renderer.drawPlayer(window, player);
        for (auto& item : items) {
            renderer.drawItemEntity(window, item);
        }

        for (const auto& item : items) {
            sf::RectangleShape itemHitboxR(item.getHitbox().size);
            itemHitboxR.setPosition(item.getHitbox().position);
            itemHitboxR.setFillColor(sf::Color::Transparent);
            itemHitboxR.setOutlineColor(sf::Color::Yellow);
            itemHitboxR.setOutlineThickness(1.f);
            window.draw(itemHitboxR);
        }

        sf::RectangleShape itemHitboxR(items[0].getHitbox().size);
        itemHitboxR.setPosition(items[0].getHitbox().position);
        itemHitboxR.setFillColor(sf::Color::Transparent);
        itemHitboxR.setOutlineColor(sf::Color::Yellow);
        itemHitboxR.setOutlineThickness(1.f);
        window.draw(itemHitboxR);

        sf::RectangleShape hitboxR(player.getHitbox().size);
        hitboxR.setPosition(player.getHitbox().position);
        hitboxR.setFillColor(sf::Color::Transparent);
        hitboxR.setOutlineColor(sf::Color::Red);
        hitboxR.setOutlineThickness(1.f);
        window.draw(hitboxR);

        window.setView(window.getDefaultView());

        renderer.drawInventory(window, inventory, selectedSlot, font);

        window.display();
    }

    return 0;
}