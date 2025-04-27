#include "Enemy.hpp"
#include "Projectile.hpp"


Enemy::Enemy() {
    length = 10;
    width = 10;
    setPosition({ 0, 0 });
    setMovement({ 100, 100 });
    shotCount = 0;
    burst = 2;
    delay = 0.5f;
    shotSpeeds = { 50.0f, 50.0f };
    movTypes = { 5, 0, 1, 2, 3, 4 };
}

Enemy::Enemy(float nx, float ny) {
    length = 10;
    width = 10;
    setPosition({ nx, ny });
    setMovement({ 100, 0 });
    shotCount = 0;
    burst = 1;
    delay = 0.5f;
    shotSpeeds = { 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f };
    movTypes = { 5, 0, 1, 2, 3, 4 };
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
            shared_from_this() // pass this enemy
        ));
        shotCount++;
    }
    return shots_out;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!isShown) return;

    sf::CircleShape circle;
    circle.setRadius(length / 2.0f);
    circle.setPosition(position);
    circle.setFillColor(sf::Color::White);

    target.draw(circle, states);
}

void Enemy::turnAround() {
	movement.x = -movement.x;
	movement.y = -movement.y;
}