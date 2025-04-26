#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
class Game_Object {

	int length;
	int width;
	bool isShown;
	int x;
	int y;
	Game_Object() {
		length = 1;
		width = 1;
		isShown = true;
		x = 0;
		y = 0;
	}
	Game_Object(int nx, int ny) {
		length = 1;
		width = 1;
		isShown = true;
		x = nx;
		y = ny;
	}

};
class Projectile : public sf::Drawable{
private:
	int length;
	int width;
	bool isShown;
	sf::Vector2f position;
	sf::Vector2f movement;
public:
	Projectile() {
		length = 10; // make it bigger so you can see
		width = 10;
		isShown = true;
		position.x = 0;
		position.y = 0;
		movement.x = 1;
		movement.y = 1;
	}

	Projectile(float nx, float ny) {
		length = 10;
		width = 10;
		isShown = true;
		position.x = nx;
		position.y = ny;
		movement.x = 1;
		movement.y = 1;
	}
	~Projectile() {
		// Optional: clean-up code
		// For now, nothing special to delete
	}
	void setPosition(sf::Vector2f nposition) {
		position.x = nposition.x;
		position.y = nposition.y;
	}
	sf::Vector2f getPosition() {
		return position;
	}
	void move() {
		this->setPosition(position + movement);
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		if (!isShown) return;
		sf::CircleShape circle;
		circle.setRadius(length / 2.0f);    // Circle needs a radius
		circle.setPosition(position);
		circle.setFillColor(sf::Color::Red); // Set a color if you want

		target.draw(circle, states);
	}
};