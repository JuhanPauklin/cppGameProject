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

	int gamestate = 0; // 0 = starting screen, 1 = game, 2 = game over, 3 = win screen

    // WINDOW
    sf::Vector2u window_size{ 1280,720 };
    sf::Vector2f window_sizef{ 1280.0f,720.0f };

    sf::RenderWindow window(sf::VideoMode(window_size), "Tohoo Tonti", sf::Style::Titlebar | sf::Style::Close);

    // Text
    sf::Font font("./FontKarmaticArcade.ttf");

    sf::Text startTextTitle(font, "Tohoo Tonti!", 60);
    startTextTitle.setPosition(sf::Vector2f(window_sizef.x * 0.30f, window_sizef.y * 0.25f));

    sf::Text startTextPress(font, "press Enter key to start", 50);
    startTextPress.setPosition(sf::Vector2f(window_sizef.x * 0.14f, window_sizef.y * 0.5f));

    sf::Text startTextTutorial(font, "WASD - move\nSpace - shoot\nShift - Slow", 30);
    startTextTutorial.setPosition(sf::Vector2f(100, window_sizef.y * 0.75f));

    // Start screen loop
    while (window.isOpen() && gamestate == 0){
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
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    gamestate = 1;
            }
        }
        window.clear(sf::Color::Black);
        window.draw(startTextTitle);
        window.draw(startTextPress);
        window.draw(startTextTutorial);
        window.display();
    }


    sf::Vector2f player_start_pos{ window_sizef.x / 2, window_sizef.y * static_cast<float>(0.75) };

    auto player = std::make_shared<Player>();
    (*player).setPosition(player_start_pos);

    sf::Text healthText(font, "hp " + std::to_string((*player).getHealth()), 40);

	// Create enemies. Enemies will be added to a queue, and then spawned into the game when needed.
	// Enemies will be chosen last from the queue, so the first enemy in the queue will be the last one to be spawned.
	int killcount = 0;
	std::vector<std::shared_ptr<Enemy>> enemiesQueue{
        std::make_shared<Enemy>(50.0f, 50.0f, 20, 3, std::vector<int>{-1, -2, 5}, std::string("./sprites/tontKolju32.png")),
        std::make_shared<Enemy>(50.0f, 50.0f, 20, 3, std::vector<int>{-2, 5, 5}, std::string("./sprites/tontKombits32.png")),
        std::make_shared<Enemy>(50.0f, 100.0f, 20, 3, std::vector<int>{-2, -2, 5}, std::string("./sprites/tontKolmik32.png")),
        std::make_shared<Enemy>(50.0f, 50.0f, 20, 3, std::vector<int>{-2, -1, 0}, std::string("./sprites/tontKolmik32.png")),
		std::make_shared<Enemy>(50.0f, 50.0f, 20, 1, std::vector<int>{0}, std::string("./sprites/tont32.png")),
        std::make_shared<Enemy>(1230.0f, 100.0f, 20, 1, std::vector<int>{0}, std::string("./sprites/tont32.png")),
        std::make_shared<Enemy>(50.0f, 50.0f, 20, 1, std::vector<int>{0}, std::string("./sprites/tont32.png")),
	};
	int zigzagEnemy = enemiesQueue.size() - 2; // Second to last enemy in the queue is a zigzag enemy
    int boss = enemiesQueue.size() - 1;

    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> allProjectiles;
    std::vector<std::shared_ptr<Projectile>> allPlayerProjectiles;

    sf::Clock fpsClock;
    sf::Clock playerShotClock;
    window.setFramerateLimit(100);
    int frameCount = 0;
    sf::Clock clock;

    // Game Loop
    while (window.isOpen() && gamestate == 1)
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

        // Keep player within window bounds
        sf::Vector2f playerPos = (*player).getPosition();
        if (playerPos.x < 0) playerPos.x = 0;
        if (playerPos.y < 0) playerPos.y = 0;
        if (playerPos.x > window_sizef.x - (*player).getRadius() * 2) {
            playerPos.x = window_sizef.x - (*player).getRadius() * 2;
        }
        if (playerPos.y > window_sizef.y - (*player).getRadius() * 2) {
            playerPos.y = window_sizef.y - (*player).getRadius() * 2;
        }
        (*player).setPosition(playerPos);

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
                    player->setHealth(player->getHealth()-1);
                    it = allProjectiles.erase(it);
					if (player->getHealth() <= 0) {
						gamestate = 2; // Game over state
					}
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
                std::vector<std::shared_ptr<Projectile>> newProjectiles = (*it)->update(*player);
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

				// This is the zigzag enemy, which will move up and down
                if (killcount == zigzagEnemy && pos.y > window_sizef.y * 0.3f) {
					(*it)->setMovement(sf::Vector2f(movement.x, -50.0f)); 
				} else if (killcount == zigzagEnemy && pos.y < window_sizef.y * 0.1f) {
					(*it)->setMovement(sf::Vector2f(movement.x, 50.0f)); 
                }
				// The boss (final enemy) also zigzags
                if (killcount == boss && pos.y > window_sizef.y * 0.6f) {
                    (*it)->setMovement(sf::Vector2f(movement.x, -50.0f));
                }
                else if (killcount == boss && pos.y < window_sizef.y * 0.2f) {
                    (*it)->setMovement(sf::Vector2f(movement.x, 50.0f));
                }


                if (isOutOfBounds(pos) || (*it)->getHealth() <= 0) {
                    it = enemies.erase(it);
					killcount++;
                }
                else {
                    ++it;
                }
            }
            else {
                it = enemies.erase(it);
            }
        }

		if (enemies.empty() && enemiesQueue.size() > 0) {
			// Add new enemy from queue
			auto newEnemy = enemiesQueue.back();
			enemiesQueue.pop_back();
			enemies.push_back(newEnemy);

            if (killcount == 1 && enemiesQueue.size() > 0) { // Second wave (after killing first enemy) has two enemies
                auto newEnemy = enemiesQueue.back();
                enemiesQueue.pop_back();
                enemies.push_back(newEnemy);
            }
		}
        // Check if all enemies are defeated
        if (enemies.empty() && enemiesQueue.empty()) {
            gamestate = 3; // Win state
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

        // FPS Counter
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            fpsClock.restart();
        }
    }

    sf::Text GameOverText(font, "Game Over", 50);
    GameOverText.setPosition(sf::Vector2f(window_sizef.x * 0.4f, window_sizef.y / 2));

    // Game over screen
    while (window.isOpen() && gamestate == 2) {
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
        window.clear(sf::Color::Black);
        window.draw(GameOverText);
        window.display();
    }

	// Victory screen
    sf::Text VictoryText(font, "Tondid on alistatud!", 50);
    VictoryText.setPosition(sf::Vector2f(window_sizef.x * 0.21f, window_sizef.y / 2));

    sf::Text VictoryYippiee(font, "Yippiee!", 50);
    VictoryYippiee.setPosition(sf::Vector2f(740, 100));

    sf::Texture victoryTexture;
    if (!victoryTexture.loadFromFile("./sprites/fumoPlush.png")) {
        std::cerr << "Error loading texture fumoPlush" << std::endl;
    }
    sf::Sprite victorySprite(victoryTexture);
    victorySprite.setPosition(sf::Vector2f((window_sizef.x - 163.0f )/ 2.0f , 50));

    while (window.isOpen() && gamestate == 3) {
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
        window.clear(sf::Color::Black);
        window.draw(VictoryText);
        window.draw(VictoryYippiee);
        window.draw(victorySprite);
        window.display();
    }


    // End of application
    return 0;
}
