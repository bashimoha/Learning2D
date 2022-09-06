#include "Game.h"

GameScene::GameScene(Engine* engine, const std::string& level_path)
	:Scene(engine), mLevelPath(level_path)
{
	mSerializer = new SceneSerializer(this);
	Init(mLevelPath);
	mEngine->Window().setTitle("Fart Scene");
}
GameScene::~GameScene() 
{
	delete mSerializer;
	delete mWorld;
	mSerializer = nullptr;
	mWorld = nullptr;
}

void GameScene::Init(const std::string& level)
{
	//Regiester key presses
	registerAction(sf::Keyboard::Space, "UP");
	registerAction(sf::Keyboard::D, "Right");
	registerAction(sf::Keyboard::A, "Left");
	registerAction(sf::Keyboard::S, "Down");
	registerAction(sf::Keyboard::Escape, "Quit");
	
	mSerializer->Deserialize(level);
	LoadPlayer();
	//Load level
	mLocal.restart();
	//setup physics world 
	mWorld = new b2World(b2Vec2(0.0f, 9.8f));
}


void GameScene::Update(sf::Clock deltaClock)
{
	mEntities.update();
	/*auto& tc = mPlayer->getComponent<CTransform>();
	tc.position = tc.velocity * deltaClock.getElapsedTime().asSeconds();*/
	//get mouse position and print it
	mLocal = deltaClock;
	Movement();
	Collision();
	sAnimation();
}
void GameScene::Collision()
{
	//do some simple AABB collision detection between mPlayer and every other entity that has a CBoundingBox component
	auto& transform = mPlayer->getComponent<CTransform>();
	auto& g = mPlayer->getComponent<CGravity>();
	for (const auto& e : mEntities.getEntities())
	{
		if( e == mPlayer)
			continue;
		if (e->hasComponent<CBoundingBox>())
		{
			vec2 overlap;
			auto collided = collision_overlap(e, mPlayer, false, overlap);
			if (collided)
			{
				std::cout << "Collision" << std::endl;
			}
		}
	}

}

void GameScene::Render()
{
	mEngine->Window().clear();
	for (auto e : mEntities.getEntities())
	{
		auto& t = e->getComponent<CTransform>();
			if (e->hasComponent<CAnimation>())
			{
				auto& entity = e->getComponent<CAnimation>();
				entity.animation.GetSprite().setRotation(t.angle);
				entity.animation.GetSprite().setPosition(t.position.x, t.position.y);
				entity.animation.GetSprite().setScale(t.scale.x, t.scale.y);
				mEngine->Window().draw(entity.animation.GetSprite());
			}
	}
}

void GameScene::DoAction(const Action& action)
{
	auto type = action.Type();
	auto name = action.Name();
	vec2 vel{0, 0};

	if (type == "BEGIN")
	{
		//move player
		if (name == "UP")
		{
			vel.y = -10;
		}
		if (name == "Down")
		{
			vel.y = 10;
		}
		if (name == "Left")
		{
			vel.x = -10;
		}
		if (name == "Right")
		{
			vel.x = 10;
		}
		if (name == "Quit")
		{
			delete mSerializer;
			mSerializer = nullptr;
			mEngine->Window().close();
		}
		
		
	}
	if (type == "END")
	{
	}
	if (type == "MOUSEMOVE")
	{
	}


	auto& tc = mPlayer->getComponent<CTransform>();
	auto sec = mLocal.getElapsedTime().asSeconds();
	tc.velocity = vel * 1/sec;
}

void GameScene::Movement()
{
	auto& tc = mPlayer->getComponent<CTransform>();
	tc.prev_position = tc.position;
	tc.position +=  tc.velocity;
}

void GameScene::LoadPlayer()
{
	//the player starts at the lower left corner
	auto player = mEntities.addEntity("player");

	player->addComponent<CTransform>(vec2(190, 545),vec2(0,0), 0);
	auto& anime = player->addComponent<CAnimation>(mEngine->GetAsset().getAnimation( 
		"idle"
	), true);
	//add Collision box
	auto& bb = player->addComponent<CBoundingBox>(anime.animation.GetSize());
	bb.collidable = true;
	mPlayer = player;
}

void GameScene::sAnimation()
{
	//update animations for all entities that have a CAnimation component
	for (auto& e : mEntities.getEntities())
	{
		if (e->hasComponent<CAnimation>())
		{
			//get the current animation
			auto& animation_component = e->getComponent<CAnimation>();
			if (animation_component.repeat)
			{
				auto scale = e->getComponent<CTransform>().scale;
				animation_component.animation.GetSprite().setScale(
					scale.x, scale.y
				);
				animation_component.animation.Update();
			}
			else
			{
				if (animation_component.animation.IsDone())
				{
					e->destroy();
				}
				else {
					animation_component.animation.Update();
				}

			}
		}
	}

}
