#pragma once  
#include "Game_Object.hpp" 
#include <SFML/System.hpp>  
#include <memory>  
#include <vector>  

class Projectile; // forward declaration  

class Player : public GameObject, public std::enable_shared_from_this<Player> {
private:
	int length;
	int width;
	float radius;
	sf::Vector2f position;
	int health;
public:
	Player();
	~Player();

	float getRadius() {
		return radius;
	}

	int getHealth() {
		return health;
	}

	void setHealth(int nhealth) {
		health = nhealth;
	}

	sf::Vector2f getPosition() {
		return position;
	}

	void setPosition(sf::Vector2f nposition) {
		position = nposition;
	}

	sf::Vector2f getMovement();
	void setMovement(sf::Vector2f nmovement);
	void move(sf::Vector2f deltaMovement);

	std::vector<std::shared_ptr<Projectile>> shoot();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};