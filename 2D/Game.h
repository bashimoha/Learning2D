#pragma once

#include "Common.h"
#include "Entity.h"
#include "EntityManager.h"



/*


*/
struct PlayerConfig {

	int ShapeRadius{0};
	int CollisionRadius{0};
	float Speed{0.f};
	sf::Color FillColor{};
	sf::Color OutlineColor{};
	int OutlineThickness{0};
	int ShapeVertices{0};
};
struct EnemyConfig {
	
	int ShapeRadius{0};
	int CollisionRadius{0};
	float MinSpeed{};
	float MaxSpeed{};
	sf::Color OutlineColor{};
	int OutlineThickness{};
	int MinShapeVertices{};
	int MaxShapeVertices{};
	int smallHealth{};
	int SpawnInterval{};
};

struct BulletConfig {
	int ShapeRadius{};
	int CollisionRadius{};
	float Speed{};
	sf::Color FillColor{};
	sf::Color OutlineColor{};
	int OutlineThickness{};
	int ShapeVertices{};
	int Health{};
};


class Game
{
public:
	Game(const std::string& config);
	void run();
private:
	sf::RenderWindow mWindow;
	EntityManager mEntities;
	PlayerConfig mPlayerConfig;
	EnemyConfig mEnemyConfig;
	BulletConfig mBulletConfig;
	std::random_device random_seed{};
		
	sf::Font mFont;
	sf::Text mText;
	size_t mPlayerScore{0};
	bool mPause{false};
	bool mRunning{true};
	size_t mFrame{0};
	size_t mLastEnemySpawnTime{ 0 };
	
	std::shared_ptr<Entity> mPlayer;

	void Init(const std::string& config);
	void setPause(bool pause);

	//sys
	void sysMovement();
	void sysInput();
	void sysRender();
	void sysCollision();
	void sysSpawnEnemy();
	void sysHealth();
	
	//helper
	void spawnEnemy();
	void spawnPlayer();
	void spawnSmallerEnemy(std::shared_ptr<Entity> enemy);
	void spawnBullet();
	void spawnPowerup(std::shared_ptr<Entity> player);
	
	void movePlayer();
	void moveBullet(std::shared_ptr<Entity> bullet);
	void moveEnemy(std::shared_ptr<Entity> enemy);

	//check for collisions helper
	bool entityWallCollided(const std::shared_ptr<Entity>& entity);
	bool entityCollided(const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2);


};