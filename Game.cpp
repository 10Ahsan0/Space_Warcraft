#include "Game.h"
#include"Global.h"
#include "Asteroid.h"
#include "Player.h"
#include <fstream>

std::vector<Entity*> Game::entities{};
std::list<std::vector<Entity*>::const_iterator> Game::toRemoveList{};
std::list<Entity*> Game::toAddList{};

sf::SoundBuffer Game::shootSoundBuffer{};
sf::Sound Game::shootSound{};

size_t Game::score{};
size_t Game::highScore;

size_t Game::incontrol{};
float Game::asteroidSpawnTime{};
Game::State Game::state{};

sf::Font Game::font{};
sf::Text Game::scoreText{};

sf::Text Game::gameOverText{};
sf::Text Game::continueText{};

sf::Text Game::highScoreText{};
sf::Text Game::titleText{};
sf::Text Game::menuText{};
sf::Text Game::playText{};

void Game::init() {
	std::ifstream file("score.dat", std::ios::binary | std::ios::in);
	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(&highScore), sizeof(size_t));
		file.close();
	}

	font.loadFromFile("font.ttf");
	scoreText.setFont(font);
	scoreText.setPosition(sf::Vector2f(40, 20));
	scoreText.setCharacterSize(48);

	gameOverText.setFont(font);
	gameOverText.setPosition(sf::Vector2f(350, 350));
	gameOverText.setCharacterSize(128);
	gameOverText.setString("Game Over!");

	continueText.setFont(font);
	continueText.setPosition(sf::Vector2f(100, 550));
	continueText.setCharacterSize(20);
	continueText.setString("Press E / M / H to continue...W,D,A to Move Forword, Move Right, move Left and C to clear High_Score");

	highScoreText.setFont(font);
	highScoreText.setPosition(sf::Vector2f(40, 20));
	highScoreText.setCharacterSize(48);
	highScoreText.setString("High Score: " + std::to_string(highScore));

	menuText.setFont(font);
	menuText.setPosition(sf::Vector2f(420, 600));
	menuText.setCharacterSize(24);
	menuText.setString("Press ESCAPE to exit to menu...");

	titleText.setFont(font);
	titleText.setPosition(sf::Vector2f(120, 350));
	titleText.setCharacterSize(128);
	titleText.setString("SPACE WARCRAFT");

	playText.setFont(font);
	playText.setPosition(sf::Vector2f(100, 550));
	playText.setCharacterSize(20);
	playText.setString("Press E / M / H to play..W,D,A to Move Forword, Move Right, move Left and C to clear High_Score");

	shootSoundBuffer.loadFromFile("shoot.wav");
	shootSound.setBuffer(shootSoundBuffer);

	state = MENU;
}

void Game::begin(int level1) {
	state = PLAYING;
	
	entities.push_back(new Player());
	if (level1 == 1) {
		//ASTEROID_SPAWN_TIME += 2.2f;
		asteroidSpawnTime = ASTEROID_SPAWN_TIME+0.7f;
		incontrol = 1;
	}
	else if (level1==2) {
		//constexpr float ASTEROID_SPAWN_TIME = 1.5f;
		asteroidSpawnTime = ASTEROID_SPAWN_TIME;
		incontrol = 2;
	}
	else if (level1 == 3) {
		//constexpr float ASTEROID_SPAWN_TIME = 0.90f;
		asteroidSpawnTime = ASTEROID_SPAWN_TIME -0.6f;
		incontrol = 3;
	}
	else if (level1 == 4) {
		highScore = 0;
		asteroidSpawnTime = ASTEROID_SPAWN_TIME;
		incontrol = 4;
		
	}

	//asteroidSpawnTime = ASTEROID_SPAWN_TIME;
	
	score = 0;
}

void Game::update(sf::RenderWindow& window, float deltaTime) {
	if (state == MENU) {
		window.draw(highScoreText);
		window.draw(titleText);
		window.draw(playText);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			begin(1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
			begin(2);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			begin(3);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			//begin(4);
			highScore = 0;
			std::ofstream file("score.dat", std::ios::binary | std::ios::out);
			if (file.is_open()) {
				file.write(reinterpret_cast<const char*>(&highScore), sizeof(size_t));
				file.close();
			}
			else {
				printf("Failed to write high score to file!\n");
			}

			highScoreText.setString("High Score: " + std::to_string(highScore));
			state = MENU;
		}
		return;
	}

	toAddList.clear();
	toRemoveList.clear();
	asteroidSpawnTime -= deltaTime;

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->update(deltaTime);
		entities[i]->render(window);
	}

	for (const auto& it : toRemoveList) {
		delete* it;
		entities.erase(it);
	}

	for (auto& ptr : toAddList) {
		entities.push_back(ptr);
	}

	if (asteroidSpawnTime <= 0.0f) {
		entities.push_back(new Asteroid());
		if (incontrol == 1) {
			//constexpr float ASTEROID_SPAWN_TIME = 1.7f;
			asteroidSpawnTime = 2.2f;
		}
		else if (incontrol == 2|| incontrol==4) {
			//constexpr float ASTEROID_SPAWN_TIME = 1.3f;
			asteroidSpawnTime = 1.5f;
		}
		else if (incontrol == 3) {
			//constexpr float ASTEROID_SPAWN_TIME = 0.90f;
			asteroidSpawnTime = 0.9f;
		}
	
		//asteroidSpawnTime = ASTEROID_SPAWN_TIME;
		
		//asteroidSpawnTime = ASTEROID_SPAWN_TIME;
		
	}

	scoreText.setString(std::to_string(score));
	window.draw(scoreText);

	if (state == GAME_OVER) {
		entities.clear();
		window.draw(gameOverText);
		window.draw(continueText);
		window.draw(menuText);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			begin(1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
			begin(2);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			begin(3);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			//begin(4);
			highScore = 0;
			std::ofstream file("score.dat", std::ios::binary | std::ios::out);
			if (file.is_open()) {
				file.write(reinterpret_cast<const char*>(&highScore), sizeof(size_t));
				file.close();
			}
			else {
				printf("Failed to write high score to file!\n");
			}

			highScoreText.setString("High Score: " + std::to_string(highScore));
			state = MENU;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			state = MENU;
		}
	}
}

void Game::gameOver() {
	if (score > highScore|| incontrol == 4) {
		highScore = score;
		std::ofstream file("score.dat", std::ios::binary | std::ios::out);
		if (file.is_open()) {
			file.write(reinterpret_cast<const char*>(&highScore), sizeof(size_t));
			file.close();
		} else {
			printf("Failed to write high score to file!\n");
		}

		highScoreText.setString("High Score: " + std::to_string(highScore));
	}

	state = GAME_OVER;
}
