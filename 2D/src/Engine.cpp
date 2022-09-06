#include "Engine.h"
Engine::Engine(const std::string& path, const vec2 window_dim)
	:mWindowWidth(window_dim.x), mWindowHeight(window_dim.y)
{
	Init(path);
}
Engine::Engine(const std::string& path)
{
	Init(path);
}


void Engine::Init(const std::string& path)
{
	mWindow.create(sf::VideoMode(mWindowWidth, mWindowHeight), "SFML Engine");
	mWindow.setFramerateLimit(FPS);
	mAssets.LoadFromFile(path);
	ImGui::SFML::Init(mWindow);
	//change to the entry point: maybe a better way is to add the scene and then change it. SO, that the engine don't know anything thing about the 
	//ChangeScene("Fart", std::make_shared<Menu>(this));
}

void Engine::Update()
{
	if (mScenes[mCurrentScene])
	{
		mScenes[mCurrentScene]->Update(clock);
	}
}
void Engine::Quit()
{
	mIsRunning = false;
}
void Engine::ChangeScene(const std::string& scene_name, std::shared_ptr<Scene> scene)
{
	//change the current scene to scene
	if (scene) {
		mScenes[scene_name] = scene;
		mCurrentScene = scene_name;
	}
}

void Engine::ChangeScene(const std::string& scene_name)
{
	//change the current scene to scene
	if (mScenes[scene_name]) {
		mCurrentScene = scene_name;
	}
}

std::shared_ptr<Scene> Engine::GetCurrentScene()
{
	return mScenes[mCurrentScene];
}
 Assets& Engine::GetAsset()
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
		ImGui::SFML::ProcessEvent(mWindow, event);
		if (event.type == sf::Event::Closed)
		{
			Quit();
		}
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{


			if (mScenes.size() != 0)
			{

				//find if the current scene has an action registerd for the key and do the action if it is
				auto key = mScenes[mCurrentScene]->mActions.find(event.key.code) != mScenes[mCurrentScene]->mActions.end();
				if (key) {
					const std::string type = event.type == sf::Event::KeyPressed ? "BEGIN" : "END";
					mScenes[mCurrentScene]->doAction(Action(
						mScenes[mCurrentScene]->mActions.at(event.key.code), type));
				}
			}
		}
		//mouse press
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (mScenes.size() != 0)
			{
				//get vec2 of the mouse position
				sf::Vector2i _mouse_pos = sf::Mouse::getPosition(mWindow);
				vec2 mouse_pos = vec2(_mouse_pos.x, _mouse_pos.y);
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					mScenes[mCurrentScene]->doAction(Action("LeftClick", "BEGIN", mouse_pos));
					break;
				case sf::Mouse::Right:
					mScenes[mCurrentScene]->doAction(Action("RightClick", "BEGIN", mouse_pos));
					break;
				case sf::Mouse::Middle:
					mScenes[mCurrentScene]->doAction(Action("MiddleClick", "BEGIN", mouse_pos));
					break;
				default:
					break;
				}
				
			}
		}
		//mouse release
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (mScenes.size() != 0)
			{
				//get vec2 of the mouse position
				sf::Vector2i _mouse_pos = sf::Mouse::getPosition(mWindow);
				vec2 mouse_pos = vec2(_mouse_pos.x, _mouse_pos.y);
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					mScenes[mCurrentScene]->doAction(Action("LeftClick", "END", mouse_pos));
					break;
				case sf::Mouse::Right:
					mScenes[mCurrentScene]->doAction(Action("RightClick", "END", mouse_pos));
					break;
				case sf::Mouse::Middle:
					mScenes[mCurrentScene]->doAction(Action("MiddleClick", "END", mouse_pos));
					break;
				default:
					break;
				}
			}
		}

		
		//mouse move
		if (event.type == sf::Event::MouseMoved)
		{
			if (mScenes.size() != 0)
			{
				//get vec2 of the mouse position
				sf::Vector2i _mouse_pos = sf::Mouse::getPosition(mWindow);
				vec2 mouse_pos = vec2(_mouse_pos.x, _mouse_pos.y);
				mScenes[mCurrentScene]->doAction(Action("MOUSEMOVE", mouse_pos));
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
	ImGui::SFML::Shutdown();
}
