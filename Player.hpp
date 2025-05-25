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
	sf::Texture texture;
public:
	Player();
	~Player();

	// No need to re-implement getRadius, getHealth, getPosition, etc.
	// Just use the ones from GameObject directly!

	void setPosition(sf::Vector2f nposition) {
		position = nposition;
	}

	void move(sf::Vector2f deltaMovement) {
		position += deltaMovement;
	}

	sf::Vector2f getMovement();
	void setMovement(sf::Vector2f nmovement);

	std::vector<std::shared_ptr<Projectile>> shoot();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
