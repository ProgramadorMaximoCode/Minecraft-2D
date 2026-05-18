#pragma once
#include <SFML/Graphics.hpp>

class Player {
    private:
        sf::Sprite playerSprite;
        float speedX;
        float speedY;
        sf::FloatRect hitbox;
        sf::Vector2f hitboxOffset;

        void updateHitbox();
        

    public:
        sf::Vector2f position;
        Player(const sf::Texture& texture, const sf::Vector2f& startPosition);
        void update(float deltaTime, float gravity);
        void setPosition(const sf::Vector2f& newPosition);
        sf::Vector2f getPosition() const;
        const sf::Sprite& getSprite() const { return playerSprite; }
        const sf::FloatRect& getHitbox() const;
        sf::Vector2f getHitboxOffset() const;
        float getSpeedX() { return speedX; }
        float getSpeedY() { return speedY; }
        void setSpeedX(float newSpeedX);
        void setSpeedY(float newSpeedY);
        void jump(float deltaTime, float gravity);
        void setTexture(const sf::Texture& newTexture) {
            playerSprite.setTexture(newTexture);
        }
};
