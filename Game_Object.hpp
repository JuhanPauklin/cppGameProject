#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class GameObject : public sf::Drawable, public sf::Transformable {
protected:
	sf::Vector2f movement;
	sf::Vector2f position;
	bool isShown;
	float radius;
	int health;
public:
	GameObject() = default;
	virtual ~GameObject() = default;
	sf::Vector2f getMovement() {
		return movement;
	}
	sf::Vector2f getPosition() {
		return position;
	}
	float getRadius() {
		return radius;
	}
	void setRadius(float nRadius) {
		radius = nRadius;
	}
	int getHealth() {
		return health;
	}
	void setHealth(int nhealth) {
		health = nhealth;
	}
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};
