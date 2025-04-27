#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <cmath>
#include "Game_Object.hpp"  // Include the GameObject header
#include "Player.hpp"
class Projectile : public GameObject {
private:
    int length;
    int width;
    bool isShown;
    sf::Vector2f position;
    sf::Vector2f startPosition;
    float speed;
    int moveType;
    std::weak_ptr<GameObject> parentEnemy;
    float orbitRadius;
    float orbitAngle;
    float spiralSpeed;
    float radiusSpeed;
    bool isSpiraling;
    sf::Vector2f parent_movement;
    sf::Vector2f lastCenterPosition;

public:
    Projectile(float nx, float ny, float nspeed, int nmovType, std::shared_ptr<GameObject> enemy);
    Projectile(float nx, float ny, sf::Vector2f nmovement, std::shared_ptr<GameObject> enemy);
    ~Projectile();

    // Getter and setter
    void setPosition(sf::Vector2f newPosition);
    sf::Vector2f getPosition();

    // Override move function
    void move(float deltaTime);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void inHitbox(Player& target, std::shared_ptr<Projectile>& self);
};