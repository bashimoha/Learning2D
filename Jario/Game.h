#pragma once
#include "Entity.h"
#include "Component.h"
#include "Engine.h"
#include "Scene.h"
#include "EntityManager.h"
#include "physics.h"
class GameScene : public Scene
{
public:
	GameScene(Engine* engine, const std::string& level_path);
	void Update();
	void Render();
	void  DoAction(const Action&);

private:
	void Init(const std::string& level);
	std::string mLevelPath{};
	EntityManager mEntites;
	std::shared_ptr<Entity> mPlayer;
	vec2 mGridSize{ 64.f, 64.f };
	bool mDebugGrid{ false };
	bool mRenderTexture{ true };
	bool mRenderCollision{ false };
	sf::CircleShape mMouseCursor{};
	void Collision();
	void sAnimation();
	//movement
	void Movementent();
	void EnemySpawner();

	void LoadLevel(const std::string& level_path);
	vec2 grid_to_world_point(float x, float y, std::shared_ptr<Entity> entity);
	std::string world_to_grid_point(std::shared_ptr<Entity> entity);
	void SaveLevel(const std::string& level_path);
	void SpawnPlayer();
	void SpawnCoin(const std::shared_ptr<Entity>& e);
	void SpawnBrokenBrick(const std::shared_ptr<Entity>& e);
};

