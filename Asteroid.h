#pragma once

#include "Entity.h"

constexpr float ASTEROID_W = 90.0f;
constexpr float ASTEROID_H = 80.0f;
constexpr float ASTEROID_SPIN = 90.0f;
constexpr float ASTEROID_ACCELERATION_RATE = 17.0f;
constexpr float ASTEROID_SPEED = 100.0f;
constexpr float ASTEROID_HIT_TIME = 1.0f;
constexpr float ASTEROID_SPAWN_TIME=1.5f;

class Asteroid : public Entity {
public:
	Asteroid(sf::Vector2f direction = Asteroid::getRandomDirection(),
		sf::Vector2f position = Asteroid::getRandomPosition());

	void update(float deltaTime) override;
	void render(sf::RenderWindow& window) override;
	const sf::VertexArray& getVertexArray() const;

	static sf::Vector2f getRandomDirection();
	static sf::Vector2f getRandomPosition();

	float getLife() { return life; }

private:
	sf::VertexArray array;
	sf::Vector2f direction;

	float life;
};
