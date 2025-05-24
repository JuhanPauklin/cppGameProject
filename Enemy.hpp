#pragma once  
#include "Game_Object.hpp"  
#include <SFML/System.hpp>  
#include <memory>  
#include <vector>  
#include <string>

class Projectile; // forward declaration  

class Enemy : public GameObject, public std::enable_shared_from_this<Enemy> {  
private:  
   int length;  
   int width;   
   int burst;  
   float delay;  
   int shotCount;  
   std::vector<float> shotSpeeds;  
   std::vector<int> movTypes;  
   sf::Clock shootClock;  
   sf::Texture texture;
public:  
   Enemy();  
   Enemy(float nx, float ny);  
   Enemy(float nx, float ny, int nhealth, std::vector<int> nmovTypes, std::string textureFileName);
   ~Enemy();  
   
   sf::Vector2f getMovement();  
   void setMovement(sf::Vector2f nmovement);  
   void setPosition(sf::Vector2f nposition);  
   std::vector<std::shared_ptr<Projectile>> update();  
   std::vector<std::shared_ptr<Projectile>> shoot();  
   void move(float deltaTime);  
   void turnAround();  

protected:  
   void draw(sf::RenderTarget& target, sf::RenderStates states) const override;  
};
