#pragma once
#include <SFML/Graphics.hpp>

struct ItemEntity {
    int itemID;
    int count;

    sf::Vector2f position;
    sf::Vector2f velocity;
    ItemEntity(int id, int count, sf::Vector2f pos, sf::Vector2f vel)
        : itemID(id), count(count), position(pos), velocity(vel) {}

    void update(float dt);
    sf::FloatRect getHitbox() const;
    sf::Vector2f getPosition() const { return position; }
};