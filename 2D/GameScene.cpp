#include "GameScene.h"

GameScene::GameScene(Engine* engine, const std::string& level_path )
	:Scene(engine), mLevelPath(level_path)
{
	Init(mLevelPath);
	mGame->Window().setTitle("Fart Scene");
}
void GameScene::Init(const std::string& level)
{

	//Regiester key presses
	registerAction(sf::Keyboard::Space, "UP");
	registerAction(sf::Keyboard::D, "Right");
	registerAction(sf::Keyboard::A, "Left");
	registerAction(sf::Keyboard::S, "Down");
	registerAction(sf::Keyboard::Escape, "Quit");
	registerAction(sf::Keyboard::T, "Texture");
	registerAction(sf::Keyboard::G, "Grid");
	registerAction(sf::Keyboard::C, "Collision");
	registerAction(sf::Keyboard::P, "Pause");

	mEntites = EntityManager();
	std::ifstream file(level);

	if (!file.is_open())
	{
		std::cout << level <<"Could not open file" << std::endl;
		return;
	}
	auto grid_to_world_point = [](float x, float y, std::shared_ptr<Entity> entity, sf::RenderWindow& window)
	{
	
		int width = window.getSize().x;
		int height = window.getSize().y;
		int grid_size = 64;
		auto size = entity->getComponent<CAnimation>().animation.GetSize();
		auto x_pos = grid_size * x;
		auto y_pos = y*size.y + (size.y / 2);
		return vec2(x_pos+ grid_size/2, height-y_pos);
		
	};
	std::string type;
	while (file.good())
	{
		std::string name;
		float x, y;
		file >>type>> name >> x >> y;
		if (type == "Tile")
		{
			//example of a tile: Tile Ground 0 0
				auto e = mEntites.addEntity(type);
				e->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
				e->addComponent<CTransform>(grid_to_world_point(x, y, e, mGame->Window()), vec2(0.f,0.f), 1);
				e->addComponent<CBoundingBox>(
					e->getComponent<CAnimation>().animation.GetSize());
		}
		else if (type == "Dec")
		{
			auto e = mEntites.addEntity(type);
			e->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
			e->addComponent<CTransform>(grid_to_world_point(x, y, e, mGame->Window()), vec2(0.f, 0.f), 1);
		}
		//mEnities.update();
	}
	auto entity = mEntites.addEntity("Player");
	entity->addComponent<CAnimation>(mGame->GetAsset().getAnimation("Walking"), true);
	entity->addComponent<CTransform>(vec2(244, 244), vec2(0, 0), 1);
	entity->addComponent<CBoundingBox>(
		entity->getComponent<CAnimation>().animation.GetSize());
	entity->addComponent<CState>();
	mPlayer = entity;
	
}


void GameScene::Update()
{
	mEntites.update();
	Movementent();
	sAnimation();
	Collision();
}

void GameScene::Render()
{
	auto drawLine = [](const vec2& p1, const vec2& p2, sf::RenderWindow& window)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(p1.x, p1.y)),
			sf::Vertex(sf::Vector2f(p2.x, p2.y))
		};
		//make each line a red line
		line[0].color = sf::Color::Magenta;
		line[1].color = sf::Color::Magenta;
		
		window.draw(line, 2, sf::Lines);
	};
	
	mGame->Window().clear(sf::Color(100, 100, 255));
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
			//std::cout << e->Tag() << ":(" << t.position.x << "," << t.position.y << ")\n";
			if (e->hasComponent<CAnimation>())
			{
				auto& entity = e->getComponent<CAnimation>();
				entity.animation.GetSprite().setRotation(t.angle);
				entity.animation.GetSprite().setPosition(t.position.x, t.position.y);
				mGame->Window().draw(entity.animation.GetSprite());
			}
		}
	}
	if (mDebugGrid)
	{
		auto width = mGame->Window().getSize().x;
		auto height = mGame->Window().getSize().y;
		float leftX = mGame->Window().getView().getCenter().x - width / 2;
		float rightX = leftX + width + mGridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)mGridSize.x);
		
		for (float x = nextGridX; x < rightX; x += mGridSize.x)
		{
			drawLine(vec2(x, 0), vec2(x, height), mGame->Window());
		}
		for (float y = 0; y < height; y += mGridSize.y)
		{
			drawLine(vec2(leftX, height - y), vec2(rightX, height - y), mGame->Window());
			for (float x = nextGridX; x < rightX; x += mGridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)mGridSize.x);
				std::string yCell = std::to_string((int)y / (int)mGridSize.y);
				sf::Text t;
				t.setFont(mGame->GetAsset().getFont("Arial"));
				t.setString( xCell + "," + yCell);
				t.setPosition(x + 3, height - y - mGridSize.y + 2);
				t.setCharacterSize(12);
				mGame->Window().draw(t);
			}
		}
	}
	if (mRenderCollision)
	{
		for (auto e : mEntites.getEntities())
		{
			
			if (e->hasComponent<CBoundingBox>())
			{
				auto& t = e->getComponent<CTransform>();
				auto& entity = e->getComponent<CBoundingBox>();
				sf::RectangleShape rect;
				rect.setPosition(t.position.x, t.position.y);
				rect.setSize(sf::Vector2f(entity.size.x, entity.size.y));
				rect.setOrigin(
					entity.size.x / 2,
					entity.size.y / 2);
				//check if entity is colliding with player and if so, make it red
				if (e->Tag() == "Player")
				{
					rect.setFillColor(sf::Color(0, 0, 0, 0));
					rect.setOutlineColor(sf::Color::White);
					rect.setOutlineThickness(1);
					mGame->Window().draw(rect);
					continue;
				}
				auto overlap = collision_overlap(e, mPlayer, false);
				if (overlap.x < 0 && overlap.y < 0)
				{
					rect.setFillColor(sf::Color(255, 0, 0));
					rect.setOutlineColor(sf::Color::Red);

				}
				else
				{
					rect.setFillColor(sf::Color(0, 0, 0, 0));
					rect.setOutlineColor(sf::Color::White);
				}
				rect.setOutlineThickness(1);
				mGame->Window().draw(rect);
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
		if (name == "Texture") { mRenderTexture = !mRenderTexture; }
		else if (name == "Grid") { mDebugGrid = !mDebugGrid; }
		else if (name == "Collision") { mRenderCollision = !mRenderCollision; }
		else if (name == "Pause") { mPause = true; }
		else if (name == "Quit") { mGame->Quit(); }
		else if (name == "UP") {
			mPlayer->getComponent<CInput>().up = true;
			mPlayer->getComponent<CState>().state = "jump";
		}
		else if (name == "Right") {
			mPlayer->getComponent<CInput>().right = true;
			mPlayer->getComponent<CState>().state = "walk";
			mPlayer->getComponent<CAnimation>().animation.GetSprite().setScale(1, 1);
		}
		else if (name == "Left") {
			mPlayer->getComponent<CInput>().left = true;
			mPlayer->getComponent<CState>().state = "walk";
			mPlayer->getComponent<CAnimation>().animation.GetSprite().setScale(-1, 1);
		}
		else if (name == "Down") {
			mPlayer->getComponent<CInput>().down = true;
		}
	}
	else
	{
		if (name == "UP") {
			mPlayer->getComponent<CInput>().up = false;
		}
		else if (name == "Right") {
			mPlayer->getComponent<CInput>().right = false;
		}
		else if (name == "Left") {
			mPlayer->getComponent<CInput>().left = false;
		}
		else if (name == "Down") {
			mPlayer->getComponent<CInput>().down = false;
		}
		else
		{
		}
	}
}

void GameScene::Movementent()
{
	//move player
	auto& input = mPlayer->getComponent<CInput>();
	auto& transform = mPlayer->getComponent<CTransform>();
	transform.prev_position  = transform.position;
	vec2 velocity(0, 0);
	if (input.up)
	{
		velocity.y -= 3;

	}
	if (input.right)
	{
		velocity.x += 3;
	}
	if (input.left)
	{
		velocity.x -= 3;
	}
	if (input.down)
	{
		velocity.y += 3;
	}
	transform.velocity = velocity;
	transform.position += transform.velocity;
}

void GameScene::EnemySpawner()
{
}

void GameScene::Collision()
{
	//do some simple AABB collision detection between mPlayer and every other entity that has a CBoundingBox component
	auto& transform = mPlayer->getComponent<CTransform>();
	for (const auto& e : mEntites.getEntities())
	{
		if (e->Tag() == "Player")
		{
			continue;
		}
		if (e->hasComponent<CBoundingBox>())
		{

			const auto overlap = collision_overlap(e, mPlayer, false);
			if (overlap.x < 0 && overlap.y < 0)
			{
				transform.position = transform.prev_position;
			}

		}
	}
	
}

void GameScene::sAnimation()
{
	//update players' animation based on state
	auto& state = mPlayer->getComponent<CState>();
	auto& animation = mPlayer->getComponent<CAnimation>();
	for (auto& e : mEntites.getEntities())
	{
		if (e->hasComponent<CAnimation>())
		{
			e->getComponent<CAnimation>().animation.Update();
		}
	}

}



