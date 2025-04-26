#pragma once
#include <SFML/Graphics.hpp>

class GameObject : public sf::Drawable {
protected:
    sf::Vector2f position;
    sf::Vector2f movement;

public:
    GameObject() : position(0.0f, 0.0f) {}
    virtual ~GameObject() {}

    // Getter and setter for position
    void setPosition(sf::Vector2f newPosition) {
        position = newPosition;
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    // Getter and setter for position
    void setMovement(sf::Vector2f newMovement) {
        position = newMovement;
    }

    sf::Vector2f getMovement() const {
        return movement;
    }

    // Virtual function to be overridden by derived classes
    virtual void move(float deltaTime) = 0;

    // Pure virtual draw function that must be implemented by derived classes
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};
