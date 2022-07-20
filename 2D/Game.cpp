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
	//see if read the correct info from the config file for player by printing it out
	
	spawnPlayer();
	
	file.close();
	
}

void Game::setPause(bool pause)
{
	mPause = pause;
}

void Game::sysMovement()
{
	//move the player
	vec2 move{ };
	if (mPlayer->cInput->right)
	{
		move.x += mPlayerConfig.Speed;
		mPlayer->cInput->right = false;
	}
	if (mPlayer->cInput->left)
	{
		move.x -= mPlayerConfig.Speed;
		mPlayer->cInput->left = false;
	}
	if (mPlayer->cInput->up)
	{
		move.y -= mPlayerConfig.Speed;
		mPlayer->cInput->up = false;
	}
	if (mPlayer->cInput->down)
	{
		move.y += mPlayerConfig.Speed;
		mPlayer->cInput->down = false;
	}
	if (move.x != 0 || move.y != 0) {
		mPlayer->cTransform->velocity += move;
		mPlayer->cTransform->position += mPlayer->cTransform->velocity;
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
		//key down
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				//wasd
				case sf::Keyboard::W:
						mPlayer->cInput->up = true;
						break;
				case sf::Keyboard::S:
						mPlayer->cInput->down = true;
						break;
				case sf::Keyboard::A:
						mPlayer->cInput->left = true;
						break;
				case sf::Keyboard::D:
						mPlayer->cInput->right = true;
						break;
			}
		}
	}
}

void Game::sysRender()
{
	mWindow.clear();
	//draw player?
	mPlayer->cShape->shape.setPosition(
		mPlayer->cTransform->position.x, mPlayer->cTransform->position.y);
	mPlayer->cTransform->angle += 1.0f;
	mPlayer->cShape->shape.setRotation(
		mPlayer->cTransform->angle
	);
	mWindow.draw(mPlayer->cShape->shape);
	mWindow.display();
}

void Game::sysCollision()
{
}

void Game::sysSpawnEnemy()
{
}

void Game::sysHealth()
{
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

void Game::spawnBullet(std::shared_ptr<Entity> entity, const vec2& mousePos)
{
}

void Game::spawnPowerup(std::shared_ptr<Entity> player)
{
}
