#include "Player.h"
#include "Bullet.h"
#include "Global.h"
#include "Game.h"
#include "Asteroid.h"
#include "Physics.h"

Player::Player()
	: Entity(sf::Vector2f(500, 500), 0), array(sf::TrianglesFan/*Quads LinesStrip*/, 7), shootTimer() {
	array[0].position = sf::Vector2f(20, 0);
	array[1].position = sf::Vector2f(0, -30);
	array[2].position = sf::Vector2f(-40, -20);
	array[3].position = sf::Vector2f(-50, 0);
	array[4].position = sf::Vector2f(-40, 20);
	array[5].position = sf::Vector2f(0, 30);
	array[6].position = array[0].position;

	for (size_t i = 0; i < array.getVertexCount(); i++) {
		array[i].color = sf::Color::Red;
	}
}

void Player::update(float deltaTime) {
	shootTimer -= deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		angle -= TURN_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		angle += TURN_SPEED * deltaTime;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		float radians = angle * (M_PI / 180.0f);

		position.x += cos(radians) * PLAYER_SPEED * deltaTime;
		position.y += sin(radians) * PLAYER_SPEED * deltaTime;

		position.x = std::min(std::max(position.x, PLAYER_W / 2.0f),
			SCREEN_WIDTH - PLAYER_W / 2.0f);
		position.y = std::min(std::max(position.y, PLAYER_H / 2.0f),
			SCREEN_HEIGHT - PLAYER_H / 2.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer <= 0.0f) {
		Game::shootSound.play();
		shootTimer = SHOOT_DELAY;

		float radians = angle * (M_PI / 180.0f);

		Game::toAddList.push_back(
			new Bullet(position, sf::Vector2f(cos(radians), sin(radians))));
	}

	sf::Transform playerTransform = sf::Transform().translate(position).rotate(angle);

	for (size_t i = 0; i < Game::entities.size(); i++) {
		if (typeid(*Game::entities[i]) == typeid(Asteroid)) {
			Asteroid* asteroid = dynamic_cast<Asteroid*>(Game::entities[i]);

			if (asteroid->getLife() < ASTEROID_HIT_TIME) {
				continue;
			}

			sf::Transform asteroidTransform = sf::Transform()
				.translate(asteroid->position)
				.rotate(asteroid->angle);

			if (physics::intersects(physics::getTransformed(array, playerTransform),
				physics::getTransformed(asteroid->getVertexArray(), asteroidTransform))) {
				Game::gameOver();
			}
		}
	}
}

void Player::render(sf::RenderWindow& window) {
	window.draw(array, sf::Transform().translate(position).rotate(angle));
}

bool Player::checkIntersects(Asteroid* asteroid)
{
	return false;
}
