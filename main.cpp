#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Projectile.hpp"
#include "Enemy.hpp"
#include "Player.hpp"


int main()
{
    // Text
    sf::Font font("./Blox2.ttf");
    sf::Text healthText(font, "hp 100", 40);

    // WINDOW
    sf::Vector2u window_size{ 1280,720 };
    sf::Vector2f window_sizef{ 1280.0f,720.0f };
	sf::Vector2f player_start_pos{ window_sizef.x / 2, window_sizef.y * static_cast<float>(0.75) };
    sf::RenderWindow window(sf::VideoMode(window_size), "Tohoo Tonti", sf::Style::Titlebar | sf::Style::Close);

    auto player = std::make_shared<Player>();
    (*player).setPosition(player_start_pos);
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> allProjectiles;
    std::vector<std::shared_ptr<Projectile>> allPlayerProjectiles;

    auto enemy2 = std::make_shared<Enemy>(50.0f, 50.0f, 20, 1, std::vector<int>{0}, std::string("./sprites/tontKombits32.png"));
    enemies.push_back(enemy2);

    sf::Clock fpsClock;
    sf::Clock playerShotClock;
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
            }
        }

        sf::Vector2f movement(0.f, 0.f);
        float speed = 200.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
            speed=50.0f;
        }
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
            if (playerShotClock.getElapsedTime().asMilliseconds()>100) {
                std::vector<std::shared_ptr<Projectile>> newProjectiles = (*player).shoot();
                for (auto& p : newProjectiles) {
                    allPlayerProjectiles.push_back(p);
                }
                playerShotClock.restart();
            }
        }
        // then apply movement
        (*player).move(movement * deltaTime);
        int i = 0;
        // move all projectiles
        auto isOutOfBounds = [&](const sf::Vector2f& pos) -> bool {
            return (pos.x > window_sizef.x + 10 || pos.y > window_sizef.y + 10 ||
                pos.x < -10 || pos.y < -10);
            };

        // Move and handle all enemy projectiles
        for (auto it = allProjectiles.begin(); it != allProjectiles.end();) {
            if (*it) {
                (*it)->move(deltaTime);

                // Check collision with player
                if ((*it)->inHitbox(*player)) {
                    player->setHealth(player->getHealth()-1); // if you have getDamage()
                    it = allProjectiles.erase(it);
                    continue;
                }

                sf::Vector2f pos = (*it)->getPosition();
                if (isOutOfBounds(pos)) {
                    it = allProjectiles.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                it = allProjectiles.erase(it);
            }
        }

        // Move and handle all player projectiles
        for (auto it = allPlayerProjectiles.begin(); it != allPlayerProjectiles.end();) {
            if (*it) {
                (*it)->move(deltaTime);

                bool hit = false;

                // check collision with all enemies
                for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ++enemyIt) {
                    if (*enemyIt && (*it)->inHitbox(**enemyIt)) {
                        (*enemyIt)->setHealth((*enemyIt)->getHealth()-1);
						(*enemyIt)->setTurnRed(10); // Enemy will become red for n frames

                        hit = true;
                        break;
                    }
                }

                if (hit) {
                    it = allPlayerProjectiles.erase(it);
                    continue;
                }

                sf::Vector2f pos = (*it)->getPosition();
                if (isOutOfBounds(pos)) {
                    it = allPlayerProjectiles.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                it = allPlayerProjectiles.erase(it);
            }
        }


        // Move and handle all enemies
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (*it) {
                (*it)->move(deltaTime);
                std::vector<std::shared_ptr<Projectile>> newProjectiles = (*it)->update();
                for (auto& p : newProjectiles) {
                    allProjectiles.push_back(p);
                }

                sf::Vector2f pos = (*it)->getPosition();

                // If near window edges and moving outward, turn around
                const auto& movement = (*it)->getMovement();
                if ((pos.x > window_sizef.x * 0.95f && movement.x > 0) ||
                    (pos.x < window_sizef.x * 0.05f && movement.x < 0)) {
                    (*it)->turnAround();
                }

                if (isOutOfBounds(pos) || (*it)->getHealth() <= 0) {
                    it = enemies.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                it = enemies.erase(it);
            }
        }


		// Update health text
		healthText.setString("hp " + std::to_string((*player).getHealth()));
        //draw shapes
        window.clear(sf::Color::Black);
        window.draw(healthText);
        window.draw((*player));
        for (auto& p : allProjectiles) {
            window.draw(*p);
        }
        for (auto& p : allPlayerProjectiles) {
            window.draw(*p);
        }
        for (auto& enemy : enemies) {
            window.draw(*enemy);
        }
        window.display();

        // --- FPS Counter ---
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            //auto enemy = std::make_shared<Enemy>(50, 50);
            //enemies.push_back(enemy);
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            fpsClock.restart();
        }
    }


    // End of application
    return 0;
}
