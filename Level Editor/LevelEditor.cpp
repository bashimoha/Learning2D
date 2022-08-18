#include "LevelEditor.h"

Editor::Editor(Engine* engine)
	:Scene(engine)
{
	registerAction(sf::Keyboard::Escape, "Quit");
	registerAction(sf::Keyboard::F1, "Toggle Debug");
	Init();
}

void Editor::Init()
{
	std::string title{ "Level Editor" };
	
	auto window_size = mGame->Window().getSize();
	mGame->Window().create(sf::VideoMode(window_size.x, window_size.y), title, sf::Style::Titlebar | sf::Style::Close);
	mGame->Window().setTitle(title);

	mMouseCursor.setRadius(10.0f);
	mMouseCursor.setPointCount(40);
	mMouseCursor.setFillColor(sf::Color::Red);
	mMouseCursor.setOrigin(mMouseCursor.getRadius(), mMouseCursor.getRadius());
	auto pos = sf::Mouse::getPosition();
	mMouseCursor.setPosition(sf::Vector2f(pos.x, pos.y));
	mMouseCursor.setOutlineColor(sf::Color::Black);

}

void Editor::Update()
{
	entityManager.update();
	ImGui::SFML::Update(mGame->Window(), deltaClock.restart());
}

void Editor::Render()
{
	ImGui::ShowDemoWindow();
	if (mDebugGrid)
	{
		DrawGrid();
	}
	DrawSelectableTexture();
	//move shape
	shape.setPosition(shape_input_pos.x, shape_input_pos.y);
	for (auto e : entityManager.getEntities())
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
	
	mGame->Window().draw(mMouseCursor);
	ImGui::SFML::Render(mGame->Window());
	

}

void Editor::DoAction(const Action& action)
{
	auto name = action.Name();
	auto type = action.Type();
	if (type == "BEGIN")
	{
		if (name == "Quit")
		{
			mGame->Window().close();
		}
		if (name == "LeftClick")
		{
			//set the dragging to true for the first entity with Dragable component
			for (auto e : entityManager.getEntities())
			{
				if (e->hasComponent<CDraggable>())
				{
					if (point_inside_entity(vec2(mMouseCursor.getPosition().x, mMouseCursor.getPosition().y), e))
					{
						e->getComponent<CDraggable>().dragging = true;
						last_click = "";
						break;
					}

				}
			}
		}
		if (name == "Toggle Debug")
		{
			mDebugGrid = !mDebugGrid;
		}
	}
	else if (type == "END")
	{
		if (name == "LeftClick")
		{
			//set the dragging to false for all entities with Dragable component
			for (auto e : entityManager.getEntities())
			{
				if (e->hasComponent<CDraggable>())
				{
					e->getComponent<CDraggable>().dragging = false;
				}
			}
		}
		
	}
	else if (type == "MOUSEMOVE")
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&& last_click != "")
		{
			auto pos = sf::Mouse::getPosition();
			CreateSelectedEntity(last_click, vec2(pos.x, pos.y));

		}
		const auto pos = action.Position();
		const auto mouse_pos = sf::Vector2i(pos.x, pos.y);
		//update the mouse cursor position based on the view
		const auto delta = mGame->Window().mapPixelToCoords(mouse_pos) - mMouseCursor.getPosition();
		//set the mouse cursor position to the new position
		mMouseCursor.setPosition(mMouseCursor.getPosition() + delta);

		for (auto e : entityManager.getEntities())
		{
			if (e->hasComponent<CDraggable>())
			{
				if (e->getComponent<CDraggable>().dragging)
				{
					const auto pos = mMouseCursor.getPosition();
					e->getComponent<CTransform>().position = vec2(pos.x, pos.y);
				}
			}
		}
	}
	std::cout << action.toString() << std::endl;
	
}

void Editor::DrawGrid()
{
	auto drawLine = [](const vec2& p1, const vec2& p2, sf::RenderWindow& window)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(p1.x, p1.y)),
			sf::Vertex(sf::Vector2f(p2.x, p2.y))
		};
		//make each line a red line
		line[0].color = sf::Color::White;
		line[1].color = sf::Color::White;

		window.draw(line, 2, sf::Lines);
	};
	auto width = mGame->Window().getSize().x / TILE_SIZE.x;
	auto heght = mGame->Window().getSize().y/ TILE_SIZE.y;
	for (int i = 0; i < width; i++)
	{
		drawLine(vec2(i * TILE_SIZE.x, 0), vec2(i * TILE_SIZE.x, mGame->Window().getSize().y), mGame->Window());
	}
	for (int i = 0; i < heght; i++)
	{
		drawLine(vec2(0, i * TILE_SIZE.y), vec2(mGame->Window().getSize().x, i * TILE_SIZE.y), mGame->Window());
	}
	
}


void Editor::DrawSelectableTexture()
{
	auto pos = mGame->Window().mapPixelToCoords(sf::Mouse::getPosition(mGame->Window()));
	
	ImGui::Begin("Choose a Texture");
	ImGui::Text("Mouse Pos (x: %f), (y: %f)",pos.x, pos.y);
	int same_line = 0;
	auto size = sf::Vector2f(64, 64);
	auto padding = 10;
	auto item_per_line = 5;
	for (auto e : mGame->GetAsset().getAnimationNames())
	{
		ImGui::ImageButton(mGame->GetAsset().getAnimation(e).GetSprite(),
			size, padding);
		if (same_line < item_per_line)
		{
			ImGui::SameLine();
			same_line++;
		}
		else {
			same_line = 0;
		}
		if (ImGui::IsItemClicked())
		{
			CreateSelectedEntity( e, vec2(pos.x, pos.y));
			last_click = e;
		}
		
	}
	ImGui::End();
}

void Editor::CreateSelectedEntity(const std::string& name, const vec2& pos)
{
	//get animation from asset
	
	auto animation = mGame->GetAsset().getAnimation(name);
	//create entity
	auto entity = entityManager.addEntity(name);
	//add postion component
	entity->addComponent<CTransform>( pos, vec2(1,1), 0.f);
	//add animation component
	entity->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
	entity->addComponent<CBoundingBox>(
		entity->getComponent<CAnimation>().animation.GetSize());
	entity->addComponent<CDraggable>();
	entity->getComponent<CDraggable>().dragging = true;
}
