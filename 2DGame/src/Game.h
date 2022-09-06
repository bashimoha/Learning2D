#pragma once
#include "2D.h"
#include "box2d.h"

class GameScene : public Scene
{
public:
	GameScene(Engine* engine, const std::string& level_path);
	~GameScene() override;
	void Update(sf::Clock) override;
	void Render() override;
	void  DoAction(const Action&) override;

private:
	void Collision();
	void sAnimation();
	void Movement();
	void EnemySpawner();
	void LoadPlayer();
private:
	void Init(const std::string& level);
	std::string mLevelPath{};
	std::shared_ptr<Entity> mPlayer{};
	sf::Clock mLocal;
	b2World* mWorld;
};

