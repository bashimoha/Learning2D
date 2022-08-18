#pragma once
#include "Common.h"
#include "Scene.h"
#include "Assets.h"
#include "imgui.h"
#include "imgui-SFML.h"

class Scene;
class Engine
{
public:
	Engine(const std::string& path, const vec2);
	Engine(const std::string& path);
	void Quit();
	void ChangeScene(const std::string& scene, std::shared_ptr<Scene>);
	std::shared_ptr<Scene> GetCurrentScene();
	 Assets& GetAsset();
	sf::RenderWindow& Window();
	void Run();
private:
	std::map<std::string, std::shared_ptr<Scene> > mScenes;
	sf::RenderWindow mWindow;
	Assets mAssets{};
	bool mIsRunning{ true };
	std::string mCurrentScene{};
	void Init(const std::string& path);
	static constexpr int DEFAULT_WIDTH{ 1280 };
	static constexpr int DEFAULT_HEIGHT{ 720 };
	static constexpr int FPS { 60 };
	size_t mWindowWidth{ DEFAULT_WIDTH };
	size_t mWindowHeight{ DEFAULT_HEIGHT };
	void Update();
	void Input();

};

