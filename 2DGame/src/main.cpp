#include "2D.h"
// #include "box2d.h"

// static constexpr int SCALE = 30;
// void CreateGround( b2World& world, const vec2& pos, const vec2& size)
// {
// 	b2BodyDef groundBodyDef;
// 	groundBodyDef.position.Set(pos.x/ SCALE, pos.y / SCALE);
// 	groundBodyDef.type = b2_staticBody;
// 	b2Body* groundBody = world.CreateBody(&groundBodyDef);
// 	b2PolygonShape groundBox;
// 	groundBox.SetAsBox( size.x/ SCALE, size.y/ SCALE);
// 	b2FixtureDef FixtureDef;
// 	FixtureDef.shape = &groundBox;
// 	FixtureDef.density = 0;
// 	groundBody->CreateFixture(&FixtureDef);
	
// }
// void CreateBox(b2World& World, int MouseX, int MouseY)
// {
// 	b2BodyDef BodyDef;
// 	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
// 	BodyDef.type = b2_dynamicBody;
// 	b2Body* Body = World.CreateBody(&BodyDef);

// 	b2PolygonShape Shape;
// 	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
// 	b2FixtureDef FixtureDef;
// 	FixtureDef.density = 2.f;
// 	FixtureDef.friction = 0.7f;
// 	FixtureDef.restitution = 1.f;
// 	FixtureDef.restitutionThreshold = 100;
// 	FixtureDef.shape = &Shape;
// 	Body->CreateFixture(&FixtureDef);
// }

// int main()
// {
// 	//basic sfml window setup
// 	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
// 	window.setFramerateLimit(60);
// 	b2Vec2 Gravity(0.f, 9.8f);
// 	b2World World(Gravity);
// 	CreateGround(World, vec2(400, 600), vec2(800, 6));
// 	while (window.isOpen())
// 	{
// 		sf::Event event;
// 		while (window.pollEvent(event))
// 		{
// 			if (event.type == sf::Event::Closed)
// 				window.close();
// 			if (event.type == sf::Event::MouseButtonPressed)
// 			{
// 				if (event.mouseButton.button == sf::Mouse::Left)
// 				{
// 					CreateBox(World, event.mouseButton.x, event.mouseButton.y);
// 				}
// 			}
// 		}
// 		World.Step(1 / 60.f, 8, 3);
// 		window.clear();
// 		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
// 		{
// 			if (BodyIterator->GetType() == b2_dynamicBody)
// 			{
// 				b2Vec2 Position = BodyIterator->GetPosition();
// 				float Angle = BodyIterator->GetAngle();
// 				sf::RectangleShape Rectangle(sf::Vector2f(32.f, 32.f));
// 				Rectangle.setOrigin(16.f, 16.f);
// 				Rectangle.setPosition(Position.x * SCALE, Position.y * SCALE);
// 				Rectangle.setRotation(Angle * 180 / b2_pi);
// 				Rectangle.setFillColor(sf::Color::Red);
// 				window.draw(Rectangle);
// 			}
// 			else
// 			{
// 				b2Vec2 Position = BodyIterator->GetPosition();
// 				float Angle = BodyIterator->GetAngle();
// 				sf::RectangleShape Rectangle(sf::Vector2f(800.f, 16.f));
// 				Rectangle.setOrigin(400.f, 8.f);
// 				Rectangle.setPosition(Position.x * SCALE, Position.y * SCALE);
// 				Rectangle.setRotation(Angle * 180 / b2_pi);
// 				Rectangle.setFillColor(sf::Color::Green);
// 				window.draw(Rectangle);
// 			}
// 		}
// 		window.display();
// 	}
// }
#include "LevelEditor.h"
int main()
{
	//change directory to  2DGame
	auto cwd = std::filesystem::current_path();
	cwd.append("2DGame");
	std::filesystem::current_path(cwd);

	Engine engine("assets/config.txt");
	engine.ChangeScene("Editor", std::make_shared<Editor> (&engine));
	engine.Run();
	return 0;
}