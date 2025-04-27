#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Player : public sf::CircleShape {
private:
	int health;
public:
	Player(float radius) : sf::CircleShape(radius) {
		health = 100;
		setFillColor(sf::Color::Green);
	}

	int getHealth() {
		return health;
	}

	void setHealth(int nhealth) {
		health = nhealth;
	}

};
class GameObject : public sf::Drawable, public sf::Transformable {
protected:
	sf::Vector2f movement;
	sf::Vector2f position;
public:
	GameObject() = default;
	virtual ~GameObject() = default;
	sf::Vector2f getMovement() {
		return movement;
	}
	sf::Vector2f getPosition() {
		return position;
	}
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};
