#pragma once
#include "Game_Object.hpp"
#include <SFML/System.hpp>
#include <memory>
#include <vector>

class Projectile; // forward declaration

class Enemy : public GameObject, public std::enable_shared_from_this<Enemy> {
private:
    int length;
    int width;
    bool isShown;
    int burst;
    float delay;
    int shotCount;
    std::vector<float> shots;
    sf::Vector2f movement;
    std::vector<int> movTypes;
    sf::Vector2f position;
    sf::Clock shootClock;
public:
    Enemy();
    Enemy(float nx, float ny);
    ~Enemy();

    void setMovement(sf::Vector2f nmovement);
    sf::Vector2f getMovement();
    sf::Vector2f getPosition();
    void move(float deltaTime);
    std::vector<std::shared_ptr<Projectile>> update();
    std::vector<std::shared_ptr<Projectile>> shoot();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
