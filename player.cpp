#include "player.h"

Player::Player(const sf::Texture& texture, const sf::Vector2f& startPosition)
    : playerSprite(texture),
      position(startPosition),
      speedX(0.f),
      speedY(0.f) {
    playerSprite.setScale({0.25f, 0.25f});
    hitboxOffset = {10.f, 2.5f};
    setPosition(position);
}

void Player::update(float deltaTime, float gravity) {
    speedY += gravity * deltaTime;
    position.x += speedX * deltaTime;
    position.y += speedY * deltaTime;
    playerSprite.setPosition(position);
    updateHitbox();
    speedX = 0.f;
}

void Player::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    playerSprite.setPosition(position);
    updateHitbox();
}
sf::Vector2f Player::getPosition() const {
    return position;
}

const sf::FloatRect& Player::getHitbox() const { 
    return hitbox; 
}

sf::Vector2f Player::getHitboxOffset() const {
    return hitboxOffset;
}

void Player::setSpeedX(float newSpeedX) { 
    speedX = newSpeedX; 
}
void Player::setSpeedY(float newSpeedY) { 
    speedY = newSpeedY; 
}

void Player::updateHitbox() {
    hitbox = sf::FloatRect(
        position + hitboxOffset,
        {20.f, 77.5f}
    );
}
