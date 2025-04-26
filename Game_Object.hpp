#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
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

	Projectile(float nx, float ny, sf::Vector2f vec) {
		length = 10;
		width = 10;
		isShown = true;
		position.x = nx;
		position.y = ny;
		movement.x = vec.x;
		movement.y = vec.y;
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

class Enemy : public sf::Drawable {
private:
	int length;
	int width;
	bool isShown;
	int burst;
	float delay;
	int shotCount;
	std::vector<sf::Vector2f> shots;
	sf::Vector2f position;
	sf::Vector2f movement;
	std::vector<sf::Vector2f> movements;
	sf::Clock shootClock;
public:
	Enemy() {
		length = 10; // make it bigger so you can see
		width = 10;
		isShown = true;
		position.x = 0;
		position.y = 0;
		movement.x = 1;
		movement.y = 1;
		shotCount = 0;
		burst = 2;
		delay = 0.1f;
		shots = { sf::Vector2f(2,2), sf::Vector2f(-2,-2) };
	}

	Enemy(float nx, float ny) {
		length = 10;
		width = 10;
		isShown = true;
		position.x = nx;
		position.y = ny;
		movement.x = 1;
		movement.y = 1;
		shotCount = 0;
		burst = 2;
		delay = 0.1f;
		shots = { sf::Vector2f(2,2), sf::Vector2f(-2,-2) };
	}
	~Enemy() {
		// Optional: clean-up code
		// For now, nothing special to delete
	}
	void setMovement(sf::Vector2f nmovement) {
		movement.x = nmovement.x;
		movement.y = nmovement.y;
	}
	std::vector<std::shared_ptr<Projectile>> shoot() {
		std::vector<std::shared_ptr<Projectile>> shots_out{};
		for (int i = 0;i < burst;i++) {
			if (shotCount == shots.size()) {
				shotCount = 0;
			}
			sf::Vector2f shot = { shots.at(shotCount) };
			shots_out.push_back(std::make_shared<Projectile>(getPosition().x, getPosition().y, shot));
			shotCount++;
		}
		return shots_out;
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
	std::vector<std::shared_ptr<Projectile>> update() {
		move();
		std::vector<std::shared_ptr<Projectile>> output;
		if (shootClock.getElapsedTime().asSeconds() >= delay) {
			output = shoot();
			shootClock.restart();
		}
		return output;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		if (!isShown) return;
		sf::CircleShape circle;
		circle.setRadius(length / 2.0f);    // Circle needs a radius
		circle.setPosition(position);
		circle.setFillColor(sf::Color::White); // Set a color if you want

		target.draw(circle, states);
	}
};