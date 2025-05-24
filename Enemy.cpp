#include "Enemy.hpp"
#include "Projectile.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>

// Default enemy constructor
Enemy::Enemy() {
    length = 10;
    width = 10;
    setPosition({ 10, 10});
    setMovement({ 100, 100 });
    setRadius(5);
    setHealth(1);
    shotCount = 0;
    burst = 3;
    delay = 0.5f;
    shotSpeeds = { 50.0f, 50.0f };
    movTypes = { 5, 0, 1, 2, 3, 4 };
    if (!texture.loadFromFile("./tont24.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
}

// Default enemy with coordinates
Enemy::Enemy(float nx, float ny) {
    length = 10;
    width = 10;
    setPosition({ nx, ny });
    setMovement({ 100, 0 });
    setRadius(5);
    setHealth(1);
    shotCount = 0;
    burst = 6;
    delay = 0.5f;
    shotSpeeds = { 200.0f, 200.0f, 200.0f, 200.0f, 50.0f};
    movTypes = { 1, 2, 3, 4, 5 };
    if (!texture.loadFromFile("./tont24.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
}

// Custom enemy
Enemy::Enemy(float nx, float ny, int nhealth, std::vector<int> nmovTypes, std::string textureFileName) {
    length = 10;
    width = 10;
    setPosition({ nx, ny });
    setMovement({ 100, 0 });
    setRadius(5);
    setHealth(nhealth);
    shotCount = 0;
    burst = 6;
    delay = 0.5f;
	movTypes = nmovTypes; // How enemy's projectiles move
	for (int i = 0; i < movTypes.size(); i++) {
		if (movTypes[i] == 5) { // Spiral
			shotSpeeds[i] = 50.0f; // Default spiral speed
        }
        else {
			shotSpeeds[i] = 200.0f; // Default linear speed
        }
	}
    if (!texture.loadFromFile(textureFileName)) {
        std::cerr << "Error loading texture" << std::endl;
    }
}

Enemy::~Enemy() {}

sf::Vector2f Enemy::getMovement() { return movement; }

void Enemy::setMovement(sf::Vector2f nmovement) {
    movement = nmovement;
}

void Enemy::setPosition(sf::Vector2f nposition) {
    position = nposition;
}

void Enemy::move(float deltaTime) {
    setPosition(getPosition() + getMovement() * deltaTime);
}

std::vector<std::shared_ptr<Projectile>> Enemy::update() {
    move(0.016f);  // Call move with deltaTime
    std::vector<std::shared_ptr<Projectile>> output;

    if (shootClock.getElapsedTime().asSeconds() >= delay) {
        output = shoot();
        shootClock.restart();
    }

    return output;
}

std::vector<std::shared_ptr<Projectile>> Enemy::shoot() {
    std::vector<std::shared_ptr<Projectile>> shots_out;
    for (int i = 0; i < burst; i++) {
        if (shotCount == shotSpeeds.size()) {
            shotCount = 0;
        }
        float shotSpeed = shotSpeeds.at(shotCount);
        shots_out.push_back(std::make_shared<Projectile>(
            position.x,
            position.y,
            shotSpeed,
            movTypes.at(shotCount),
            shared_from_this()
        ));
        shotCount++;
    }
    return shots_out;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!isShown) return;

    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    target.draw(sprite, states);
}

void Enemy::turnAround() {
	movement.x = -movement.x;
	movement.y = -movement.y;
}