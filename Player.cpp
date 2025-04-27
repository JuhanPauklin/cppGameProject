#include "Projectile.hpp"
#include "Player.hpp"

Player::Player() {
    setHealth(100);
	radius = 5.0f;
    length = 10;
    width = 10;
}

Player::~Player() {}

sf::Vector2f Player::getMovement() { return movement; }

void Player::setMovement(sf::Vector2f nmovement) {
    movement = nmovement;
}

std::vector<std::shared_ptr<Projectile>> Player::shoot() {
    std::vector<std::shared_ptr<Projectile>> shots_out;
    std::vector<sf::Vector2f> movement = {{ 0.0f, -100.0f },{-6.0f, -100.0f},{6.0f,-100.0f}};
    shots_out.push_back(std::make_shared<Projectile>(
        position.x,
        position.y,
        5.0f,
        movement.at(0),
        shared_from_this() // pass the player obj
    ));
    shots_out.push_back(std::make_shared<Projectile>(
        position.x,
        position.y,
        5.0f,
        movement.at(1),
        shared_from_this() // pass the player obj
    ));
    shots_out.push_back(std::make_shared<Projectile>(
        position.x,
        position.y,
        5.0f,
        movement.at(2),
        shared_from_this() // pass the player obj
    ));

    return shots_out;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    //if (!isShown) return;

    sf::CircleShape circle;
    circle.setRadius(5.0f);
    circle.setPosition(position);
    circle.setFillColor(sf::Color::Green);

    target.draw(circle, states);
}