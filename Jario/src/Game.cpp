#if 0
#include "Game.h"

GameScene::GameScene(Engine* engine, const std::string& level_path)
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
	registerAction(sf::Keyboard::K, "Duplicate");


	//set the mMouseCursor circle to some default values
	mMouseCursor.setRadius(10.0f);
	mMouseCursor.setPointCount(40);
	mMouseCursor.setFillColor(sf::Color::Red);
	mMouseCursor.setOrigin(mMouseCursor.getRadius(), mMouseCursor.getRadius());
	auto pos = sf::Mouse::getPosition();
	mMouseCursor.setPosition(sf::Vector2f(pos.x, pos.y));
	mMouseCursor.setOutlineColor(sf::Color::Black);
	mEntites = EntityManager();
	LoadLevel(level);
	SpawnPlayer();
}


void GameScene::Update()
{
	mEntites.update();
	Movementent();
	Collision();
	sAnimation();
	mCurrentFrame++;
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
	auto window_size_x = std::max((float)mGame->Window().getSize().x / 2, transform.position.x);
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
	if (mDebugGrid)
	{
		auto width = mGame->Window().getSize().x;
		auto height = mGame->Window().getSize().y;
		float leftX = mGame->Window().getView().getCenter().x - width / 2;
		float rightX = leftX + width + mGridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)mGridSize.x);

		for (int x = nextGridX; x < rightX; x += mGridSize.x)
		{
			drawLine(vec2(x, 0), vec2(x, height), mGame->Window());
		}
		for (int y = 0; y < height; y += mGridSize.y)
		{
			drawLine(vec2(leftX, height - y), vec2(rightX, height - y), mGame->Window());
			for (int x = nextGridX; x < rightX; x += mGridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)mGridSize.x);
				std::string yCell = std::to_string((int)y / (int)mGridSize.y);
				sf::Text t;
				t.setFont(mGame->GetAsset().getFont("Arial"));
				t.setString(xCell + "," + yCell);
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
				vec2 overlap;
				bool collided = collision_overlap(mPlayer, mPlayer, false, overlap);
				if (!collided)
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
	// draw the mouse cursor
	mGame->Window().draw(mMouseCursor);
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
		else if (name == "Quit") {
			//save the game
			SaveLevel(mLevelPath);
			mGame->Quit(); 
		}
		else if (name == "UP") {
			mPlayer->getComponent<CInput>().up = true;
		}
		else if (name == "Right") {
			mPlayer->getComponent<CInput>().right = true;
		}
		else if (name == "Left") {
			mPlayer->getComponent<CInput>().left = true;
		}
		else if (name == "Down") {
			mPlayer->getComponent<CInput>().down = true;
		}
		if (name == "LeftClick")
		{
			//set the dragging to true for the first entity with Dragable component
			for (auto e : mEntites.getEntities())
			{
				if (e->hasComponent<CDraggable>())
				{
					if (point_inside_entity(vec2(mMouseCursor.getPosition().x, mMouseCursor.getPosition().y), e))
					{
						e->getComponent<CDraggable>().dragging = true;
						break;
					}
				
				}
			}
		}
		if (name == "Duplicate")
		{
			for (auto e : mEntites.getEntities())
			{
				if (e->hasComponent<CDraggable>())
				{
					if (point_inside_entity(vec2(mMouseCursor.getPosition().x, mMouseCursor.getPosition().y), e))
					{
						e->getComponent<CDraggable>().duplicate = true;
						break;
					}

				}
			}
		}
	}
	if (type == "END")
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
		if (name == "LeftClick")
		{
			//set the dragging to false for all entities with Dragable component
			for (auto e : mEntites.getEntities())
			{
				if (e->hasComponent<CDraggable>())
				{
					e->getComponent<CDraggable>().dragging = false;
				}
			}
		}
		if (name == "Duplicate")
		{
			for (auto e : mEntites.getEntities())
			{
				if (e->hasComponent<CDraggable>() && e->getComponent<CDraggable>().duplicate)
				{
					e->getComponent<CDraggable>().duplicate = false;
					// create a new copy of the current entity
					auto n = mEntites.addEntity( e->Tag() );
					
					 Entity::CopyComponentFrom(e, n);
					 break;
				}
			}
		}
		
	}
	if (type == "MOUSEMOVE")
	{
		const auto pos = action.Position();
		const auto mouse_pos = sf::Vector2i(pos.x, pos.y);
		//update the mouse cursor position based on the view
		const auto delta = mGame->Window().mapPixelToCoords(mouse_pos) - mMouseCursor.getPosition();
		//set the mouse cursor position to the new position
		mMouseCursor.setPosition(mMouseCursor.getPosition() + delta);


		// if the mouse is over an entity with a draggable component, update the position of the entity
		for (auto e : mEntites.getEntities())
		{
			if (e->hasComponent<CDraggable>())
			{
				if (e->getComponent<CDraggable>().dragging)
				{
					const auto pos = mMouseCursor.getPosition();
					e->getComponent<CTransform>().position = vec2(pos.x, pos.y);
					break;
				}
			}
		}
	}
}

void GameScene::Movementent()
{
	//move player
	auto& input = mPlayer->getComponent<CInput>();
	auto& transform = mPlayer->getComponent<CTransform>();
	transform.prev_position = transform.position;
	vec2 velocity(0, 0);
	//maybe we shouldn't change the animation here and do it in the animation system but naah too lazy
	auto& s = mPlayer->getComponent<CState>();
	if (input.up && !input.right && !input.left)
	{
		//check if already in air
		if (transform.velocity.y >= 0 || s.state != "Jumping")
		{
			velocity.y -= 32;
		}
		if (s.state != "Jumping")
		{
			s.state = "Jumping";
			mPlayer->addComponent<CAnimation>(mGame->GetAsset().getAnimation("Jump64"), true);
		}
		auto& g = mPlayer->getComponent<CGravity>();
		g.gravity = g.gravity_velocity;
	}
	if (input.right)
	{
		velocity.x += 4; // idk why this is 4 but it is

		transform.scale = vec2(1, 1);
		if (s.state != "Walking")
		{
			s.state = "Walking";
			mPlayer->addComponent<CAnimation>(mGame->GetAsset().getAnimation("MarioWalking"), true);
		}
	}
	if (input.left)
	{
		velocity.x -= 4; // idk why this is 4 but it is
		transform.scale = vec2(-1, 1);
		if (s.state != "Walking")
		{
			s.state = "Walking";
			mPlayer->addComponent<CAnimation>(mGame->GetAsset().getAnimation("MarioWalking"), true);
		}
	}
	if (!input.left && !input.right && !input.up && mPlayer->getComponent<CGravity>().gravity != 3)
	{
		mPlayer->addComponent<CAnimation>(mGame->GetAsset().getAnimation("Stand64"), true);
		s.state = "Standing";
	}
	//apply gravity to the y velocity
	velocity.y += mPlayer->getComponent<CGravity>().gravity;
	//apply friction of 0.9 to the x component of player
	velocity.x *= 0.9f;
	transform.velocity = velocity;
	transform.position += transform.velocity;
}

void GameScene::EnemySpawner()
{
}

void GameScene::LoadLevel(const std::string& level_path)
{
	std::ifstream file(level_path);

	if (!file.is_open())
	{
		std::cout << level_path << "Could not open file" << std::endl;
		return;
	}

	std::string type;
	while (file.good())
	{
		std::string name;
		float x, y;
		file >> type >> name >> x >> y;
		if (type == "Tile")
		{
			//example of a tile: Tile Ground 0 0
			auto e = mEntites.addEntity(type);
			e->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
			e->addComponent<CTransform>(grid_to_world_point(x, y, e), vec2(0.f, 0.f), 1);
			e->addComponent<CBoundingBox>(
				e->getComponent<CAnimation>().animation.GetSize());
			e->addComponent<CDraggable> ();
		}
		else if (type == "Dec")
		{
			auto e = mEntites.addEntity(type);
			e->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
			e->addComponent<CTransform>(grid_to_world_point(x, y, e), vec2(0.f, 0.f), 1);
			e->addComponent<CDraggable>();
		}
	}

}
//convert level postion to world position (pixels)
vec2 GameScene::grid_to_world_point(float x, float y, std::shared_ptr<Entity> entity)
{
	int width = mGame->Window().getSize().x;
	int height = mGame->Window().getSize().y;
	auto size = entity->getComponent<CAnimation>().animation.GetSize();
	auto x_pos = mGridSize.x * x;
	auto y_pos = y * size.y + (size.y / 2);
	return vec2(x_pos + mGridSize.x / 2, height - y_pos);
}
// convert all the entity position to a level postion for saving
// in the format TYPE NAME X Y where x, y is the position in the level grid
std::string GameScene::world_to_grid_point(std::shared_ptr<Entity> entity)
{
	std::stringstream ss;
	int width = mGame->Window().getSize().x;
	int height = mGame->Window().getSize().y;
	auto animation = entity->getComponent<CAnimation>().animation;
	auto size = animation.GetSize();
	auto& transform = entity->getComponent<CTransform>();
	auto x_pos = (transform.position.x - (size.x / 2)) / mGridSize.x;
	auto y_pos = (height - transform.position.y - (size.y / 2)) / mGridSize.y;
	
	ss << entity->Tag()<< " " <<animation.GetName() << " " << x_pos << " " << y_pos;
	return ss.str();
}

void GameScene::SaveLevel(const std::string& level_path)
{
	//create new file and write to it
	auto new_file = "save.txt";
	std::ofstream file(new_file, std::ios_base::out);
	if (!file.is_open())
	{
		std::cout << "Could not open file" << std::endl;
		return;
	}
	for (auto& e : mEntites.getEntities())
	{
		file << world_to_grid_point( e) << std::endl;
	}
	file.close();
}

void GameScene::SpawnPlayer()
{
	auto entity = mEntites.addEntity("Player");
	entity->addComponent<CAnimation>(mGame->GetAsset().getAnimation("Stand64"), true);
	entity->addComponent<CTransform>(vec2(244, 244), vec2(0, 0), 1);
	entity->addComponent<CBoundingBox>(
		entity->getComponent<CAnimation>().animation.GetSize());
	entity->addComponent<CState>();
	entity->addComponent<CInput>();
	entity->addComponent<CGravity>(3.f);

	mPlayer = entity;
}
//generate coin above e's position+gridSize
void GameScene::SpawnCoin(const std::shared_ptr<Entity>& e)
{
	auto coin = mEntites.addEntity("Coin");
	//print the e's position
	auto x = e->getComponent<CTransform>().position.x;
	auto y = e->getComponent<CTransform>().position.y;
	//spawn the coin above the e's position
	coin->addComponent<CTransform>(vec2(x, y - (mGridSize.y + 10)), vec2(0, 0), 1);
	coin->addComponent<CAnimation>(mGame->GetAsset().getAnimation("Coin"), true);
	coin->addComponent<CBoundingBox>(
		coin->getComponent<CAnimation>().animation.GetSize());
}

//try to spawn broken bricks ontop of current brick
void GameScene::SpawnBrokenBrick(const std::shared_ptr<Entity>& e)
{
	auto brick = mEntites.addEntity("BrokenBrick");
	//print the e's position
	auto x = e->getComponent<CTransform>().position.x;
	auto y = e->getComponent<CTransform>().position.y;
	//spawn the coin above the e's position
	brick->addComponent<CTransform>(vec2(x, y), vec2(0, 0), 1);
	brick->addComponent<CAnimation>(mGame->GetAsset().getAnimation("BrokenBrick"), false);
	brick->addComponent<CBoundingBox>(
		brick->getComponent<CAnimation>().animation.GetSize());
}

void GameScene::Collision()
{
	//do some simple AABB collision detection between mPlayer and every other entity that has a CBoundingBox component
	auto& transform = mPlayer->getComponent<CTransform>();
	auto& g = mPlayer->getComponent<CGravity>();
	for (const auto& e : mEntites.getEntities("Tile"))
	{
		if (e->Tag() == "Player")
		{
			continue;
		}
		if (e->hasComponent<CBoundingBox>())
		{
			vec2 overlap;
			auto collided = collision_overlap(e, mPlayer, false, overlap);
			if (collided)
			{
				vec2 previous_overlap;
				auto c = collision_overlap(e, mPlayer, true, previous_overlap);
				auto& animation = e->getComponent<CAnimation>().animation;
				if (previous_overlap.y > 0)
				{
				}
				if (previous_overlap.x > 0)
				{
					//if it's y is higher it come from the top
					if (transform.position.y < e->getComponent<CTransform>().position.y)
					{
						//set gravity to 0
						g.gravity = 0;
						if (animation.GetName() == "Question")
						{
							//try to destroy the coin if it's a question block
							try {
								auto coins = mEntites.getEntities("Coin");
								for (auto& coin : coins) {
									//if the coin is above the question block and they have the same x's then destroy the coin
									auto coin_pos = coin->getComponent<CTransform>().position;
									auto question_pos = e->getComponent<CTransform>().position;
									if (coin_pos.y < question_pos.y && coin_pos.x == question_pos.x) {
										coin->destroy();
									}
								}
							}
							catch (std::exception& e)
							{
								std::cout << e.what() << std::endl;
							}
						}
					}
					else
					{
						//set gravity to 3
						g.gravity = 3;
						//get e's animation name
						// spawn a Coin  gridSize.y above the question block
						if (animation.GetName() == "Question" && animation.GetFrameCount() != 1)
						{
							//set the animation to the first frame
							animation.SetFrame(1);
							SpawnCoin(e);
						}
						//if name is "Brick" spawn broken brick and destory the current brick
						if (animation.GetName() == "Brick")
						{
							SpawnBrokenBrick(e);
							e->destroy();
						}

					}
				}
				transform.position = transform.prev_position;
			}
			if (!collided)
			{
				//if state is idle set gravity to 3
				if (mPlayer->getComponent<CState>().state == "Standing")
				{
					g.gravity = 3;
				}
			}
		}
	}

}

void GameScene::sAnimation()
{
	//update animations for all entities that have a CAnimation component
	for (auto& e : mEntites.getEntities())
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
#endif
