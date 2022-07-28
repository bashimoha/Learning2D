#include "Engine.h"

Engine::Engine(const std::string& path)
{
	Init(path);
}

void Engine::Init(const std::string& path)
{
	mAssets.LoadFromFile(path);
	mWindow.create( sf::VideoMode(800, 600), "SFML Engine" );
	mWindow.setFramerateLimit(60);
}

void Engine::Update()
{
    mScenes[mCurrentScene]->Update();
}
void Engine::Quit()
{
	mIsRunning = false;
}
void Engine::ChangeScene(const std::string& scene)
{
    mCurrentScene = scene;
}
std::shared_ptr<Scene> Engine::GetCurrentScene()
{
    return mScenes[mCurrentScene];
}
Assets Engine::GetAsset() const
{
	return mAssets;
}
sf::RenderWindow& Engine::Window() 
{
	return mWindow;
}

void Engine::Input()
{
	//process input
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Quit();
		}
		if( event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased )
		{
			//get registerd keyboard keys and create new Action based
			auto key = mScenes[mCurrentScene]->ActionMap().find(event.key.code);
			if (key == mScenes[mCurrentScene]->ActionMap().end())
			{
				continue;
			}
			const std::string type = event.type == sf::Event::KeyPressed ? "BEGIN" : "END";

			mScenes[mCurrentScene]->doAction(Action(
				mScenes[mCurrentScene]->ActionMap().at(event.key.code), type));
			
		}
	}
	
}
