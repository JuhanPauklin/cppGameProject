#pragma once
#include <iostream>
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

class Projectile : public sf::Drawable, public sf::Transformable {
private:
	int length;
	int width;
	bool isShown;
	sf::Vector2f position;
	sf::Vector2f movement;
public:
	Projectile() {
		length = 10;
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

	void inHitbox(Player& target, std::shared_ptr<Projectile>& self) {
		sf::Vector2f targetPos = target.getPosition();
		float targerRadius = target.getRadius() * 2;
		if (position.x > targetPos.x && position.x < targetPos.x + targerRadius &&
			position.y > targetPos.y && position.y < targetPos.y + targerRadius) {
			target.setHealth(target.getHealth() - 1);
			self.reset();
		}
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		if (!isShown) return;
		sf::CircleShape circle;
		circle.setRadius(length / 2.0f);
		circle.setPosition(position);
		circle.setFillColor(sf::Color::Red);

		target.draw(circle, states);
	}
};

class Enemy : public sf::Drawable, public sf::Transformable {
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
		length = 10;
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
		circle.setRadius(length / 2.0f);   
		circle.setPosition(position);
		circle.setFillColor(sf::Color::White);

		target.draw(circle, states);
	}
};