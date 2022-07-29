#include "Engine.h"
#include "MenuScene.h"
Engine::Engine(const std::string& path)
{
	Init(path);
}

void Engine::Init(const std::string& path)
{
	mAssets.LoadFromFile(path);
	mWindow.create( sf::VideoMode(800, 600), "SFML Engine" );
	mWindow.setFramerateLimit(60);
	ChangeScene("Fart", std::make_shared<Menu>(this));
}

void Engine::Update()
{
	if (mScenes[mCurrentScene])
	{
		mScenes[mCurrentScene]->Update();
	}
}
void Engine::Quit()
{
	mIsRunning = false;
}
void Engine::ChangeScene(const std::string& scene_name, std::shared_ptr<Scene> scene)
{
	//change the current scene to scene
	if (scene){
		mScenes[scene_name] = scene;
		mCurrentScene = scene_name;
	}
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
			

			if (mScenes.size() != 0)
			{
				
				//get registerd keyboard keys and create new Action based

				auto key = mScenes[mCurrentScene]->mActions.find(event.key.code) != mScenes[mCurrentScene]->mActions.end();
				if (key) {
					const std::string type = event.type == sf::Event::KeyPressed ? "BEGIN" : "END";
					mScenes[mCurrentScene]->doAction(Action(
						mScenes[mCurrentScene]->mActions.at(event.key.code), type));
				}
			}
		}
	}
	
}
void Engine::Run()
{
	mIsRunning = true;
	while (mIsRunning)
	{
		Input();
		Update();
		mWindow.clear();
		if (mScenes[mCurrentScene])
		{
		mScenes[mCurrentScene]->Render();
		}
		mWindow.display();
	}
}
