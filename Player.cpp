#include "Projectile.hpp"
#include "Player.hpp"

Player::Player() {
	health = 100;
	radius = 5.0f;
    length = 10;
    width = 10;
}

Player::~Player() {}

sf::Vector2f Player::getMovement() { return movement; }

void Player::setMovement(sf::Vector2f nmovement) {
    movement = nmovement;
}
void Player::move(sf::Vector2f deltaMovement) {
    position += deltaMovement;
}

std::vector<std::shared_ptr<Projectile>> Player::shoot() {
    std::vector<std::shared_ptr<Projectile>> shots_out;

    shots_out.push_back(std::make_shared<Projectile>(
        position.x,
        position.y,
        50.0f,
        1,
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