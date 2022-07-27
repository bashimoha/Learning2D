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
		//print true if mPause is true
		if (!mPause)
		{
			sysSpawnEnemy();
			sysCollision();
			sysMovement();
			sysHealth();
		}
		sysInput();
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
	file >> temp >> width >> height >> max_fps >> Fullscreen;
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

	//read enemy info
	file>>temp>> mEnemyConfig.ShapeRadius
		>> mEnemyConfig.CollisionRadius
		>> mEnemyConfig.MinSpeed
		>> mEnemyConfig.MaxSpeed
		>>r>>g>>b
		>> mEnemyConfig.OutlineThickness
		>> mEnemyConfig.MinShapeVertices
		>> mEnemyConfig.MaxShapeVertices
		>>mEnemyConfig.smallHealth
		>>mEnemyConfig.SpawnInterval;
	mEnemyConfig.OutlineColor = sf::Color(r, g, b);
	file.close();
}

void Game::setPause(bool pause)
{
	mPause = pause;
}

void Game::sysMovement()
{
	for (auto e : mEntities.getEntities())
	{
		if (e->Tag() == "player")
		{
			movePlayer();
		}
		else if (e->Tag() == "bullet")
		{
			moveBullet(e);
		}
		else
		{
			moveEnemy(e);
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
			case sf::Keyboard::P:
				if (event.type == sf::Event::KeyPressed)
				{
					setPause(true);
				}
				else
				{
					setPause(false);
				}
			}
		}
		//mouse down || mouse up
		if (event.type == sf::Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				spawnBullet();
				break;
			}
		}
	}
}

void Game::sysRender()
{
	mWindow.clear();

	for (auto e : mEntities.getEntities())
	{
		e->cTransform->angle += 1.0f;
		e->cShape->shape.setRotation(
			e->cTransform->angle);
		mWindow.draw(e->cShape->shape);
	}
	mWindow.display();
}

void Game::sysCollision()
{
	//make sure all the entities don't go out of bounds
	for (auto e : mEntities.getEntities())
	{
		//if the entity is out of bounds, move it back in
		if (e->Tag() == "enemy") {
			if (entityWallCollided(e)[0] || entityWallCollided(e)[1])
			{
				e->cTransform->velocity.x = e->cTransform->velocity.x *-1;
			}
			if (entityWallCollided(e)[2] || entityWallCollided(e)[3])
			{
				e->cTransform->velocity.y = e->cTransform->velocity.y *-1;
			}
			//collides with player it dies
			if (entityCollided(e, mPlayer))
			{
				e->destroy();
				//respawn player
				mPlayer->destroy();
				spawnPlayer();
			}
			//if enemy colides with bullet enemy get destroyed
			for (auto b : mEntities.getEntities())
			{
				if (b->Tag() == "bullet")
				{
					if (entityCollided(e, b))
					{
						e->destroy();
						b->destroy();
						spawnSmallerEnemy(e);
					}
				}
			}
		}
	}
	
	
}

void Game::sysSpawnEnemy()
{
	//only spawn enemies if the lastspawn time is greater than the spawn interval
	if (mFrame - mLastEnemySpawnTime > mEnemyConfig.SpawnInterval)
	{
		//spawn an enemy
		spawnEnemy();
		//reset the last spawn time
		mLastEnemySpawnTime = mFrame;
	}
}

void Game::sysHealth()
{
	for (auto e : mEntities.getEntities())
	{
		if (!e->cHealth) { continue; }
		if (e->cHealth->health <= 0)
		{
			e->destroy();
		}
		else
		{
			e->cHealth->health -= 1;
			//change the alpha color of the shape to represent the health
			auto color = e->cShape->shape.getFillColor();
			
			
		}
	}
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

void Game::spawnEnemy()
{
	auto entity = mEntities.addEntity("enemy");
	std::mt19937 rng(random_seed());
	std::uniform_int_distribution<int> xDist( 0+mEnemyConfig.ShapeRadius,(int) mWindow.getSize().x- mEnemyConfig.ShapeRadius);
	std::uniform_int_distribution<int> yDist(
		0 + mEnemyConfig.ShapeRadius, (int) mWindow.getSize().y - mEnemyConfig.ShapeRadius);
	std::uniform_int_distribution<int> verticesDist(mEnemyConfig.MinShapeVertices, mEnemyConfig.MaxShapeVertices);
	std::uniform_int_distribution<int> colorDist(0, 255);
	std::uniform_int_distribution<int> speedDist(mEnemyConfig.MinSpeed, mEnemyConfig.MaxSpeed);
	auto x = xDist(rng);
	auto y = yDist(rng);
	
	
	//attach transform component
	entity->cTransform = std::make_shared<CTransform>(
		vec2(xDist(rng), yDist(rng))
		, vec2(speedDist(rng), speedDist(rng)),
		0);
	//attach shape component
	entity->cShape = std::make_shared<CShape>
		(mEnemyConfig.ShapeRadius, verticesDist(rng),
			sf::Color(colorDist(rng), colorDist(rng), colorDist(rng)),
			mEnemyConfig.OutlineColor,
			mEnemyConfig.OutlineThickness);
	//attach collision component to the entity
	entity->cCollision = std::make_shared<CCollision>(mEnemyConfig.CollisionRadius);

}
void Game::spawnSmallerEnemy(std::shared_ptr<Entity> enemy)
{
	auto N_enemies = enemy->cShape->shape.getPointCount();
	auto angle = std::_Pi*2 / N_enemies;
	
	
	for (int i= 0; i < N_enemies; i++)
	{
		auto entity = mEntities.addEntity("enemy");
		//the postition(x, y) of the smaller enemy is the same as the postion of the bigger enemy partioned into n_part. where n_part is 360/Bigenemy.vertices.
		auto x = enemy->cTransform->position.x + cos(angle * i) * enemy->cShape->shape.getRadius();
		auto y = enemy->cTransform->position.y + sin(angle*i) * enemy->cShape->shape.getRadius();
		auto dir = vec2(cos(angle * i) * enemy->cShape->shape.getRadius(), sin(angle * i) * enemy->cShape->shape.getRadius()).normalize();
		
		entity->cTransform = std::make_shared<CTransform>(
			vec2(x, y), dir * mEnemyConfig.MinSpeed
			, 0);
		entity->cShape = std::make_shared<CShape>
			(enemy->cShape->shape.getRadius()/2, N_enemies,
				enemy->cShape->shape.getFillColor(), mEnemyConfig.OutlineColor, mEnemyConfig.OutlineThickness);
		entity->cCollision = std::make_shared<CCollision>(mEnemyConfig.CollisionRadius);
		entity->cHealth = std::make_shared<CHealth>(mEnemyConfig.smallHealth);
		
	}
}

void Game::spawnBullet()
{
	//create new entity
	auto bullet = mEntities.addEntity("bullet");
	//get the the mouse position and player position
	auto mousePos = sf::Mouse::getPosition(mWindow);
	auto playerPos = mPlayer->cTransform->position;
	//get the angle between the two points
	auto angle = atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x);
	//get the velocity of the bullet
	auto velocity = vec2(cos(angle), sin(angle)).normalize() * mBulletConfig.Speed;
	//attach transform component
	bullet->cTransform = std::make_shared<CTransform>(
		playerPos,
		velocity,
		angle);
	//attach the shape to the entity
	bullet->cShape = std::make_shared<CShape>
		(mBulletConfig.ShapeRadius, mBulletConfig.ShapeVertices,
			mBulletConfig.FillColor, mBulletConfig.OutlineColor, mBulletConfig.OutlineThickness);
	//attach the collision component to the entity
	bullet->cCollision = std::make_shared<CCollision>(mBulletConfig.CollisionRadius);
	//attach the health component to the entity
	bullet->cHealth = std::make_shared<CHealth>(mBulletConfig.Health);
}

void Game::spawnPowerup(std::shared_ptr<Entity> player)
{
	
}

void Game::movePlayer()
{
	//update player velocity based on input(wasd)
	auto speed = mPlayerConfig.Speed;
	vec2 player_velocity = { 0, 0 };
	mPlayer->cTransform->angle += 1.0f;
	mPlayer->cShape->shape.setRotation(
		mPlayer->cTransform->angle);

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
	auto player_pos = mPlayer->cTransform->position;
	mPlayer->cTransform->position += mPlayer->cTransform->velocity;
	if (entityWallCollided(mPlayer).any())
	{
		mPlayer->cTransform->position = player_pos;
	}
	
	//update the shape position to match the transform position
	mPlayer->cShape->shape.setPosition(
		mPlayer->cTransform->position.x,
		mPlayer->cTransform->position.y);
}

void Game::moveBullet(std::shared_ptr<Entity> bullet)
{
	bullet->cTransform->position += bullet->cTransform->velocity;
	//update the shape position to match the transform position
	//check if player is out of bound

	
	bullet->cShape->shape.setPosition(bullet->cTransform->position.x, bullet->cTransform->position.y);
	bullet->cShape->shape.setRotation(bullet->cTransform->angle);
}

void Game::moveEnemy(std::shared_ptr<Entity> enemy)
{
	enemy->cTransform->position += enemy->cTransform->velocity;
	enemy->cShape->shape.setPosition(enemy->cTransform->position.x, enemy->cTransform->position.y);
	enemy->cShape->shape.setRotation(enemy->cTransform->angle);
}

//checks if entity is out of bound and returns bitset<4> where[0] =left, [1] = right, [2] = top, [3] = bottom
std::bitset<4> Game::entityWallCollided(const std::shared_ptr<Entity>& entity)
{
	std::bitset<4> collided;  
	auto window_dimension = mWindow.getSize();
	
	if (entity->cTransform->position.x - entity->cShape->shape.getRadius() < 0)
	{
		collided[0] = true;
	}
	if (entity->cTransform->position.x + entity->cShape->shape.getRadius() > window_dimension.x)
	{
		collided[1] = true;
	}
	if (entity->cTransform->position.y - entity->cShape->shape.getRadius() < 0)
	{
		collided[2] = true;
	}
	if (entity->cTransform->position.y + entity->cShape->shape.getRadius() > window_dimension.y)
	{
		collided[3] = true;
	}
	return collided;
}

bool Game::entityCollided(const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2)
{
	auto position_diff = entity1->cTransform->position - entity2->cTransform->position;
	auto distance = position_diff.x * position_diff.x + position_diff.y * position_diff.y;
	auto radius_sum = entity1->cShape->shape.getRadius() + entity2->cShape->shape.getRadius();
	return distance < radius_sum* radius_sum;
}