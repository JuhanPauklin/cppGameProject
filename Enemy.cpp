#include "Enemy.hpp"
#include "Projectile.hpp"

Enemy::Enemy() {
    length = 10;
    width = 10;
    isShown = true;
    position = { 0, 0 };
    movement = { 100, 100 };
    shotCount = 0;
    burst = 2;
    delay = 0.5f;
    shots = { 50.0f, 50.0f };
    movTypes = { 5, 5 };
}

Enemy::Enemy(float nx, float ny) {
    length = 10;
    width = 10;
    isShown = true;
    position = { nx, ny };
    movement = { 100, 100 };
    shotCount = 0;
    burst = 2;
    delay = 0.5f;
    shots = { 50.0f, 50.0f };
    movTypes = { 5, 5 };
}

Enemy::~Enemy() {}

void Enemy::setMovement(sf::Vector2f nmovement) {
    movement = nmovement*100.0f;
}

sf::Vector2f Enemy::getMovement()
{
    return movement;
}

sf::Vector2f Enemy::getPosition()
{
    return position;
}

void Enemy::move(float deltaTime) {
    position += movement * deltaTime;
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
        if (shotCount == shots.size()) {
            shotCount = 0;
        }
        float shot = shots.at(shotCount);
        shots_out.push_back(std::make_shared<Projectile>(
            position.x,
            position.y,
            shot,
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
