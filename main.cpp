#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Game_Object.hpp"
int main()
{
    // WINDOW

    sf::Vector2u window_size{ 640,480 };
    sf::RenderWindow window(sf::VideoMode(window_size), "SFML works!", sf::Style::Titlebar | sf::Style::Close);

    sf::CircleShape shape(5.f);
    shape.setFillColor(sf::Color::Green);
    std::vector<std::shared_ptr<Projectile>> projectiles;
    // Game Loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                sf::Vector2f move;
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                    move.x = 0;
                    move.y = 5;
                    shape.move(move);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
                    move.x = -5;
                    move.y = 0;
                    shape.move(move);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                    move.x = 0;
                    move.y = -5;
                    shape.move(move);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::D) {
                    move.x = 5;
                    move.y = 0;
                    shape.move(move);
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    auto projectile = std::make_shared<Projectile>(50, 50); // ✅ make shared pointer
                    projectiles.push_back(projectile);
                }
            }
        }
        //move
        window.clear(sf::Color::Black);
        window.draw(shape);
        int i = 0;
        for (int i = projectiles.size() - 1; i >= 0; --i) {
            projectiles[i]->move();
            if (projectiles[i]->getPosition().x >= window_size.x
                || projectiles[i]->getPosition().y >= window_size.y
                || projectiles[i]->getPosition().x < 0
                || projectiles[i]->getPosition().y < 0) {
                projectiles.erase(projectiles.begin() + i); // Erase using iterator
            }
            else {
                window.draw(*projectiles[i]);
            }
        }
        window.display();
        
    }

    // End of application
    return 0;
}
