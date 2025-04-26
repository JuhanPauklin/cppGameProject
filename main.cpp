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
    // Text
    sf::Font font("./Blox2.ttf");
    sf::Text healthText(font, "hp 100", 40);

    // WINDOW
    sf::Vector2u window_size{ 640,480 };
    sf::RenderWindow window(sf::VideoMode(window_size), "SFML works!", sf::Style::Titlebar | sf::Style::Close);

    Player player(5.f);
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> allProjectiles;

    sf::Clock fpsClock;
    window.setFramerateLimit(100);
    int frameCount = 0;
    sf::Clock clock;
    // Game Loop
    while (window.isOpen())
    {
        sf::Time dt = clock.restart(); // restart clock and get elapsed time
        float deltaTime = dt.asSeconds(); // get seconds as a float
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
            movement.y -= speed; // move up
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            movement.y += speed; // move down
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            movement.x -= speed; // move left
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            movement.x += speed; // move right
        }

        // then apply movement
        player.move(movement * deltaTime);
        int i = 0;
        for (int i = enemies.size() - 1; i >= 0; --i) {
            std::shared_ptr<Enemy> enemy = enemies.at(i);
            // update enemy
            std::vector<std::shared_ptr<Projectile>> newProjectiles = enemy->update();
            for (auto& p : newProjectiles) {
                allProjectiles.push_back(p);
            }
        }
        // move all projectiles
        for (auto it = allProjectiles.begin(); it != allProjectiles.end();) {
            if (*it) {
                (*it)->move();
                (*it)->inHitbox(player, *it); // If in hitbox, remove projectile
                if (!*it) {
                    it = allProjectiles.erase(it); // Remove the null pointer from the vector
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }

		// Update health text
		healthText.setString("hp " + std::to_string(player.getHealth()));
        //draw shapes
        window.clear(sf::Color::Black);
        window.draw(healthText);
        window.draw(player);
        for (auto& p : allProjectiles) {
            window.draw(*p);
        }
        for (auto& enemy : enemies) {
            window.draw(*enemy);
        }
        window.display();
        // Count frame
        frameCount++;

        // If 1 second passed, output FPS
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            fpsClock.restart();
        }
        
    }

    // End of application
    return 0;
}
