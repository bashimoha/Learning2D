#include "Game.h"
#include "Entity.h"



Game::Game(const std::string& config)
{
	Init(config);
}

void Game::run()
{
	while (mRunning)
	{
		mEntities.update();
		sysSpawnEnemy();
		sysCollision();
		sysInput();
		sysHealth();
		sysMovement();
		sysRender();
		mFrame++;
		
	}
}

void Game::Init(const std::string& config)
{
	//read config file
	std::ifstream file(config);
	if (!file.is_open())
	{
		std::cerr << "Error opening config file" << std::endl;
		return;
	}
	//read the window dimension:
	size_t width, height, max_fps, Fullscreen;
	std::string temp;
	file >>temp >> width >> height >> max_fps >> Fullscreen;
	mWindow.create(sf::VideoMode(width, height), "fart", sf::Style::Close | sf::Style::Resize);

	mWindow.setFramerateLimit(max_fps);
	
	//read the players settings

	int r, g, b;
	
	file >> temp >> mPlayerConfig.ShapeRadius
		>> mPlayerConfig.CollisionRadius
		>> mPlayerConfig.Speed
		>> r >> g >> b;
	mPlayerConfig.FillColor = sf::Color(r, g, b);
	file >> r >> g >> b
		>> mPlayerConfig.OutlineThickness
		>> mPlayerConfig.ShapeVertices;
	mPlayerConfig.OutlineColor = sf::Color(r, g, b);
	spawnPlayer();

	//read bullet info
	file >> temp >> mBulletConfig.ShapeRadius
		>> mBulletConfig.CollisionRadius
		>> mBulletConfig.Speed
		>> r >> g >> b;
	mBulletConfig.FillColor = sf::Color(r, g, b);
	file >> r >> g >> b
		>> mBulletConfig.OutlineThickness
		>> mBulletConfig.ShapeVertices
		>> mBulletConfig.Health;
	mBulletConfig.OutlineColor = sf::Color(r, g, b);
	file.close();
}

void Game::setPause(bool pause)
{
	mPause = pause;
}

void Game::sysMovement()
{
	for(auto e: mEntities.getEntities())
	{
		if (e->Tag() == "player")
		{
			movePlayer();
		}
		else if (e->Tag() == "bullet")
		{
			float x = sf::Mouse::getPosition().x, y = sf::Mouse::getPosition().y;
			moveBullet(e, {x, y});
		}
	}
}


void Game::sysInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mRunning = false;
		}
		//key down || key up
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
				//toggle the pressed state
				case sf::Keyboard::W:
					mPlayer->cInput->up = !(mPlayer->cInput->up);
					break;
				case sf::Keyboard::S:
					mPlayer->cInput->down = !(mPlayer->cInput->down);
					break;
				case sf::Keyboard::A:
					mPlayer->cInput->left = !(mPlayer->cInput->left);
						break;
				case sf::Keyboard::D:
					mPlayer->cInput->right = !(mPlayer->cInput->right);
						break;
			}
		}
		//mouse down || mouse up
		if (event.type == sf::Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
				case sf::Mouse::Left:
					spawnBullet( vec2(mPlayer->cTransform->position.x, mPlayer->cTransform->position.y));
					break;
			}
		}
	}
}

void Game::sysRender()
{
	mWindow.clear();
	mPlayer->cTransform->angle += 1.0f;
	mPlayer->cShape->shape.setRotation(
		mPlayer->cTransform->angle
	);

	for (auto e : mEntities.getEntities())
	{
		e->cShape->shape.setPosition(
			e->cTransform->position.x, e->cTransform->position.y);
	}
	for(auto e: mEntities.getEntities())
	{
		mWindow.draw(e->cShape->shape);
	}
	mWindow.display();
}

void Game::sysCollision()
{
	//make sure all the entities don't go out of bounds
	for (auto e : mEntities.getEntities())
	{
		if (e->cTransform->position.x-e->cShape->shape.getRadius() < 0)
		{
			if (e->Tag() == "player") {
				e->cTransform->position.x = std::abs(e->cTransform->position.x + e->cShape->shape.getRadius());
				e->destroy();
			}
		}
		if (e->cTransform->position.x + e->cShape->shape.getRadius() > mWindow.getSize().x)
		{
			if (e->Tag() == "player") {
				e->cTransform->position.x = std::abs(e->cTransform->position.x - e->cShape->shape.getRadius());
			}
		}
		if (e->cTransform->position.y - e->cShape->shape.getRadius() < 0)
		{
			if (e->Tag() == "player") {
				e->cTransform->position.y = std::abs(e->cTransform->position.y + e->cShape->shape.getRadius());
			}
		}
		if (e->cTransform->position.y + e->cShape->shape.getRadius() > mWindow.getSize().y)
		{
			if (e->Tag() == "player") {
				e->cTransform->position.y = std::abs(e->cTransform->position.y - e->cShape->shape.getRadius());
			}
		}
		
	}
}

void Game::sysSpawnEnemy()
{
}

void Game::sysHealth()
{
	for (auto e : mEntities.getEntities())
	{
		if (!e->cHealth) { continue; }
		if(e->cHealth->health <= 0)
		{
			std::cout << e->cHealth->health << std::endl;
			e->destroy();
		}
		else
		{
			e->cHealth->health -= 1;
			std::cout << e->cHealth->health << std::endl;
			//change the alpha color of the shape to represent the health
			auto color = e->cShape->shape.getFillColor();
			color.a = (e->cHealth->health / mBulletConfig.Health) * 255;
			e->cShape->shape.setFillColor(color);
		}
	}
}

void Game::spawnEnemy()
{
}

void Game::spawnPlayer()
{
	auto entity = mEntities.addEntity("player");
	//attach transform component
	entity->cTransform = std::make_shared<CTransform>(
		vec2(mWindow.getSize().x / 2, mWindow.getSize().y / 2)
		, vec2(mPlayerConfig.Speed, mPlayerConfig.Speed),
		0);
	//attach the shape to the entity
	entity->cShape = std::make_shared<CShape>
		(mPlayerConfig.ShapeRadius, mPlayerConfig.ShapeVertices,
			mPlayerConfig.FillColor, mPlayerConfig.OutlineColor, mPlayerConfig.OutlineThickness);

	//attach the collision component to the entity
	entity->cCollision = std::make_shared<CCollision>(mPlayerConfig.CollisionRadius);

	entity->cInput = std::make_shared<CInput>();
	entity->cScore = std::make_shared<CScore>(0);
	//this goes against the factory pattern, but it's the easiet way to get the player to the game class
	mPlayer = entity;
}

void Game::spawnSmallerEnemy(std::shared_ptr<Entity> enemy)
{
	
}

void Game::spawnBullet(const vec2& mousePos)
{
	//create new entity
	auto bullet = mEntities.addEntity("bullet");
	//attach transform component
	bullet->cTransform = std::make_shared<CTransform>(
		vec2(mPlayer->cTransform->position.x, mPlayer->cTransform->position.y),
		vec2(0, 0), 0);
	//attach the shape to the entity
	/*std::mt19937 rng(random_seed());
	std::uniform_int_distribution<int> gen(mBuklletConfig.MinShapeVertices, mBulletConfig.MaxShapeVertices);
	auto shapeVertices = gen(rng);*/
	bullet->cShape = std::make_shared<CShape>
		(mBulletConfig.ShapeRadius, mBulletConfig.ShapeVertices,
			mBulletConfig.FillColor, mBulletConfig.OutlineColor, mBulletConfig.OutlineThickness);
	//attach the collision component to the entity
	bullet->cCollision = std::make_shared<CCollision>(mBulletConfig.CollisionRadius);
	//attach the input component to the entity
	bullet->cInput = std::make_shared<CInput>();
	bullet->cHealth = std::make_shared<CHealth>(mBulletConfig.Health);
}

void Game::spawnPowerup(std::shared_ptr<Entity> player)
{
}

void Game::movePlayer()
{
	//update player velocity based on input(wasd)
	auto speed = mPlayerConfig.Speed;
	vec2 player_velocity = {0, 0};

	if (mPlayer->cInput->up)
	{
		player_velocity.y = -speed;
	}
	if (mPlayer->cInput->down)
	{
		player_velocity.y = speed;
	}
	if (mPlayer->cInput->left)
	{
		player_velocity.x = -speed;
	}
	if (mPlayer->cInput->right)
	{
		player_velocity.x = speed;
	}
	mPlayer->cTransform->velocity = player_velocity;
	mPlayer->cTransform->position += mPlayer->cTransform->velocity;
}

void Game::moveBullet(std::shared_ptr<Entity> player, vec2 mousePos)
{

	//get angle between player and mouse
	auto angle = atan2(mousePos.y - player->cTransform->position.y, mousePos.x - player->cTransform->position.x);
	//update bullet velocity based on angle
	auto speed = mBulletConfig.Speed;
	vec2 bullet_velocity = {0, 0};
	bullet_velocity.x = speed * cos(angle);
	bullet_velocity.y = speed * sin(angle);
	player->cTransform->velocity = bullet_velocity;
	player->cTransform->position += player->cTransform->velocity;
}
 
