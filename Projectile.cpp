#include "Projectile.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include <iostream>

Projectile::Projectile(float nx, float ny, float nspeed, int nmovType, std::shared_ptr<GameObject> enemy)
{
    length = 10;
    width = 10;
    isShown = true;
    position = { nx, ny };
    speed = nspeed;
    moveType = nmovType;
    parentEnemy = enemy;
    startPosition = {nx,ny};
    parent_movement = enemy->getMovement();
    lastCenterPosition = startPosition;
    if (nmovType == 5) { // Spiral mode
        isSpiraling = true;
    }
    else {
        isSpiraling = false;
    }
    orbitAngle = 180.0f;
    orbitRadius = 10.0f;
    spiralSpeed = nspeed * 3.6f; // degrees per second
    radiusSpeed = nspeed;  // units per second
}

// Linear projectile constructor
Projectile::Projectile(float nx, float ny, sf::Vector2f nmovement, std::shared_ptr<GameObject> enemy) {
    length = 10;
    width = 10;
    isShown = true;
    position = { nx, ny };
    movement = nmovement;
    speed = 50.0f;
    moveType = 0;
    parentEnemy = enemy;
    startPosition = { nx,ny };
    parent_movement = enemy->getMovement();
    lastCenterPosition = startPosition;
    isSpiraling = false;
    orbitAngle = 180.0f;
    orbitRadius = 10.0f;
    spiralSpeed = 50.0f * 3.6f; // degrees per second
    radiusSpeed = 50.0f;  // units per second
}

Projectile::~Projectile() {}

void Projectile::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
}

sf::Vector2f Projectile::getPosition() {
    return position;
}

void Projectile::move(float deltaTime) {
    if (moveType == 5 && isSpiraling) {
        // Update orbit angle and radius
        orbitAngle += spiralSpeed * deltaTime;  // The orbit angle in degrees
        orbitRadius += radiusSpeed * deltaTime; // The radius increases, making the spiral bigger

        // Convert orbitAngle to radians
        float rad = orbitAngle * (3.14159265f / 180.0f);

        // Get the center of the spiral (enemy's position)
        //sf::Vector2f centerPosition = startPosition;
        sf::Vector2f centerPosition = lastCenterPosition;
        if (auto parent = parentEnemy.lock()) {
            // If the enemy is still alive, set the center to the enemy's position
            centerPosition = parent->getPosition();
        }
        else {
            centerPosition += parent_movement;
        }
        lastCenterPosition = centerPosition;
        // Calculate the new position in the spiral path
        position.x = centerPosition.x + std::cos(rad) * orbitRadius;
        position.y = centerPosition.y + std::sin(rad) * orbitRadius;
    }
    else {
        // Non-spiraling movement behavior (based on moveType)
        sf::Vector2f customMovement;
        switch (moveType) {
        case 0:
            customMovement = movement;
            break;
        case 1:
            customMovement = { speed, -speed };
            break;
        case 2:
            // Example sinusoidal motion
            customMovement.x = std::sin(position.y * 0.05f) * 200.0f * 0.05f;
            customMovement.y = speed;
            break;
        case 3:
            // Empty for now
            break;
        }
        this->setPosition(position + customMovement * deltaTime);
    }
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!isShown) return;

    sf::CircleShape circle;
    circle.setRadius(length / 2.0f);
    circle.setPosition(position);
    circle.setFillColor(sf::Color::Red);

    target.draw(circle, states);
}


void Projectile::inHitbox(Player& target, std::shared_ptr<Projectile>& self) {
    sf::Vector2f targetPos = target.getPosition();
    float targerRadius = target.getRadius() * 2;
    if (position.x > targetPos.x && position.x < targetPos.x + targerRadius &&
        position.y > targetPos.y && position.y < targetPos.y + targerRadius) {
        target.setHealth(target.getHealth() - 1);
        self.reset();
    }
}