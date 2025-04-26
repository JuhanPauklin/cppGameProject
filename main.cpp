#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Projectile.hpp"
#include "Enemy.hpp"
int main()
{
    // WINDOW

    sf::Vector2u window_size{ 640,480 };
    sf::RenderWindow window(sf::VideoMode(window_size), "SFML works!", sf::Style::Titlebar | sf::Style::Close);

    sf::CircleShape shape(5.f);
    shape.setFillColor(sf::Color::Green);
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> allProjectiles;

    sf::Clock fpsClock;
    window.setFramerateLimit(100);
    int frameCount = 0;
    sf::Clock clock;
    // Game Loop
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    auto enemy = std::make_shared<Enemy>(50, 50);
                    enemies.push_back(enemy);
                }
            }
        }

        sf::Vector2f movement(0.f, 0.f);
        float speed = 100.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            movement.y -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += speed;
        }

        shape.move(movement * deltaTime);

        // --- Update Enemies ---
        for (auto& enemy : enemies) {
            std::vector<std::shared_ptr<Projectile>> newProjectiles = enemy->update();
            for (auto& p : newProjectiles) {
                allProjectiles.push_back(p);
            }
        }

        // --- Move Projectiles ---
        for (auto& p : allProjectiles) {
            p->move(deltaTime);
        }

        // --- Delete projectiles out of bounds ---
        allProjectiles.erase(
            std::remove_if(allProjectiles.begin(), allProjectiles.end(),
                [&](const std::shared_ptr<Projectile>& p) {
                    sf::Vector2f pos = p->getPosition();
                    return pos.x < 0 || pos.x > window_size.x || pos.y < 0 || pos.y > window_size.y;
                }),
            allProjectiles.end()
        );

        // --- Delete enemies out of bounds ---
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [&](const std::shared_ptr<Enemy>& e) {
                    sf::Vector2f pos = e->getPosition();
                    return pos.x < 0 || pos.x > window_size.x || pos.y < 0 || pos.y > window_size.y;
                }),
            enemies.end()
        );

        // --- Draw ---
        window.clear(sf::Color::Black);
        window.draw(shape);

        for (auto& p : allProjectiles) {
            window.draw(*p);
        }
        for (auto& enemy : enemies) {
            window.draw(*enemy);
        }
        window.display();

        // --- FPS Counter ---
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            fpsClock.restart();
        }
    }


    // End of application
    return 0;
}
