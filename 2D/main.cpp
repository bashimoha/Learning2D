#include "Engine.h"
#include "SFML\Graphics.hpp"
int main()
{
	Engine engine("assets/test.txt");
	engine.Run();
	return 0;
}
#if 0
class Collidable
{
public:
	Collidable(const vec2& position, const vec2& size)
		: m_position(position), m_size(size)
	{
		m_rect = sf::RectangleShape(sf::Vector2f(m_size.x, m_size.y));
		m_rect.setPosition(m_position.x, m_position.y);
	}
	void Move()
	{
		m_position.x += m_velocity.x;
		m_position.y += m_velocity.y;
		m_rect.setPosition(m_position.x, m_position.y);
	}
	void Draw(sf::RenderWindow& window)
	{
		window.draw(m_rect);
	}
	void UpdateVelocity(const vec2& dir)
	{
		m_velocity += dir*m_speed;
	}
	sf::RectangleShape& GetRect()
	{
		return m_rect;
	}
private:
	sf::RectangleShape m_rect;
	vec2 m_size{};
	vec2 m_position{};
	vec2 m_velocity{};
	const float m_speed = 100.0f;
};
bool Collision(Collidable& a, Collidable& b)
{
	return a.GetRect().getGlobalBounds().intersects(b.GetRect().getGlobalBounds());
}
int main()
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(1280, 704), "SFML Engine");
	window.setFramerateLimit(60);
	Collidable Player = Collidable(vec2(1280/2, 704/2), vec2(64, 64));
	//create vector of random Collidable objects
	std::vector<Collidable> collidables;
	for (int i = 0; i < 10; i++)
	{
		//random position between 0 and 1280
		float x = rand() % 1280;
		//random position between 0 and 704
		float y = rand() % 704;
		vec2 size = { (float) (rand() % 100), (float) (rand() % 100) };
		Collidable collidable(vec2(x, y), size);
		collidables.push_back(collidable);
	}
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//draw all 
		window.clear();
		for (auto& collidable : collidables)
		{
			collidable.GetRect().setOutlineColor(sf::Color::Red);
			collidable.GetRect().setOutlineThickness(1.0f);
			collidable.Draw(window);
			
		}
		Player.GetRect().setOutlineColor(sf::Color::Green);
		Player.GetRect().setFillColor(sf::Color::Blue);
		Player.GetRect().setOutlineThickness(1.0f);
		//move player based on mouse position
		auto mousePosition = sf::Mouse::getPosition(window);
		Player.GetRect().setPosition(mousePosition.x, mousePosition.y);
		//check for collision with all collidables
		for (auto& collidable : collidables)
		{
			if (Collision(Player, collidable))
			{
				//change the color to red if it isn't red
				
				collidable.GetRect().setFillColor(sf::Color::Red);
			}
			else
			{
				collidable.GetRect().setFillColor(sf::Color::Green);
			}
		}
		
		Player.Draw(window);
		window.display();
	}
}
#endif