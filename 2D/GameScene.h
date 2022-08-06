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
	vec2 mGridSize{64.f, 64.f};
	bool mDebugGrid{ false };
	bool mRenderTexture{ true };
	bool mRenderCollision{ false };
	void Collision();
	void sAnimation();
	//movement
	void Movementent();
	void EnemySpawner();
	
	
};

