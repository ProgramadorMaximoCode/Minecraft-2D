#include "ItemEntity.h"

void ItemEntity::update(float dt) {
    velocity.y += 500 * dt;
}

sf::FloatRect ItemEntity::getHitbox() const {
    return sf::FloatRect(position, {20.f, 20.f});
}