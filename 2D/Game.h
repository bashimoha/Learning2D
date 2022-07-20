#pragma once
#include <SFML/Graphics.hpp>

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
	/*int fillR{}, fillG{}, fillB{};
	int outlineR{}, outlineG{}, outlineB{};*/
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
	int MinShapeVertices{};
	int MaxShapeVertices{};
	int Health{};
	int SpawnInterval{};
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
	void spawnBullet(std::shared_ptr<Entity> entity,const vec2& mousePos);
	void spawnPowerup(std::shared_ptr<Entity> player);
};