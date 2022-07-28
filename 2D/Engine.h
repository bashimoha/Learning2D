#pragma once
#include "Common.h"
#include "Scene.h"
#include "Assets.h"

class Scene;
class Engine
{
public:
	Engine(const std::string& path );
	void Init(const std::string& path);
	void Update();
	void Quit();
	void ChangeScene(const std::string& scene);
	std::shared_ptr<Scene> GetCurrentScene();
	Assets GetAsset() const ;
	sf::RenderWindow& Window();
	void Input();
private:
	std::map<std::string, std::shared_ptr<Scene> > mScenes;
	sf::RenderWindow mWindow;
	Assets mAssets{};
	bool mIsRunning{true};
	std::string mCurrentScene{};
};

