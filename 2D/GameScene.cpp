#include "GameScene.h"

GameScene::GameScene(Engine* engine, const std::string& level_path )
	:Scene(engine), mLevelPath(level_path)
{
	Init(mLevelPath);
	mGame->Window().setTitle("Level Scene?");
}
void GameScene::Init(const std::string& level)
{

	//Regiester key presses
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::D, "Right");
	registerAction(sf::Keyboard::A, "Left");
	registerAction(sf::Keyboard::Escape, "Quit");
	registerAction(sf::Keyboard::T, "Texture");
	registerAction(sf::Keyboard::G, "Grid");
	registerAction(sf::Keyboard::C, "Collision");

	mEntites = EntityManager();
	std::ifstream file(level);

	if (!file.is_open())
	{
		std::cout << level <<"Could not open file" << std::endl;
		return;
	}
	std::string type;
	while (file.good())
	{
		file >> type;
	}
	auto entity = mEntites.addEntity("player");
	entity->addComponent<CAnimation>(mGame->GetAsset().getAnimation("Stand64"), true);
	entity->addComponent<CTransform>(vec2(100, 100), vec2(0, 0), 1);
	entity->addComponent < CBoundingBox>(64, 64);
	mPlayer = entity;
}


void GameScene::Update()
{
	mEntites.update();
}

void GameScene::Render()
{
	mGame->Window().clear({ 50,50, 100 });
	//reset the view depeneding on player's pos
	auto& transform = mPlayer->getComponent<CTransform>();
	auto window_size_x = std::max((float)mGame->Window().getSize().x/2, transform.position.x);
	auto view = mGame->Window().getView();
	view.setCenter(window_size_x, mGame->Window().getSize().y - view.getCenter().y);
	mGame->Window().setView(view);
	
	if (mRenderTexture)
	{
		for (auto e : mEntites.getEntities())
		{
			auto& t = e->getComponent<CTransform>();
			if (e->hasComponent<CAnimation>())
			{
				auto& entity = e->getComponent<CAnimation>();
				entity.animation.GetSprite().setRotation(t.angle);
				entity.animation.GetSprite().setPosition(t.position.x, t.position.y);
				mGame->Window().draw(entity.animation.GetSprite());
			}
		}
	}
}

void GameScene::DoAction(const Action& action)
{
	auto type = action.Type();
	auto name = action.Name();
	if (type == "BEGIN")
	{
		if (name == "Texture") { mRenderTexture = true;}
		else if (name == "Grid"){ mDebugGrid = true;}
		else if (name == "Collision") { mRenderCollision = true; }
	}
	else if (type == "END")
	{
		if (name == "Texture")       { mRenderTexture = false;}
		else if (name == "Grid")     { mDebugGrid = false;}
		else if (name == "Collision"){ mRenderCollision = false; }
	}
}

void GameScene::EnemySpawner()
{
}

void GameScene::Collision()
{
}



