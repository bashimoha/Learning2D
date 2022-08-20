#include "LevelEditor.h"

Editor::Editor(Engine* engine)
	:Scene(engine)
{
	registerAction(sf::Keyboard::Escape, "Quit");
	registerAction(sf::Keyboard::G, "Debug");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::L, "Load");
	
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
	mEnities.update();
	ImGui::SFML::Update(mGame->Window(), deltaClock.restart());
}

void Editor::Render()
{
	mGame->Window().clear(sf::Color(100, 100, 255));

	if (mDebugGrid)
	{
		DrawGrid();
	}
	
	for (auto e : mEnities.getEntities())
	{
		auto& t = e->getComponent<CTransform>();
		if (e->hasComponent<CAnimation>())
		{
			auto& entity = e->getComponent<CAnimation>();
			entity.animation.GetSprite().setRotation(t.angle);
			entity.animation.GetSprite().setPosition(t.position.x, t.position.y);
			entity.animation.GetSprite().setScale(t.scale.x, t.scale.y);
			mGame->Window().draw(entity.animation.GetSprite());
		}
	}

	ImGui::ShowDemoWindow();
	//draw red border around selected entity
	if (mSelectedEntity)
	{
		DrawSelectedEntityBoarder();
	}
	DrawSelectableTexture();
	ImGui::Begin("Properties");

	if (mSelectedEntity)
	{
		PropertyWindow(mSelectedEntity);
	}
	ImGui::End();
	mGame->Window().draw(mMouseCursor);
	ImGui::SFML::Render(mGame->Window());
	

}

void Editor::DoAction(const Action& action)
{
	auto name = action.Name();
	auto type = action.Type();
	if (type == "BEGIN")
	{
		// wasd movement to move the view
		if (name == "UP")        {MoveScreenView(vec2(0.0f, -TILE_SIZE.y));}
		else if (name == "DOWN") {MoveScreenView(vec2(0.0f, TILE_SIZE.y));}
		else if (name == "LEFT") {MoveScreenView(vec2(-TILE_SIZE.x, 0.0f));}
		else if (name == "RIGHT"){MoveScreenView(vec2(TILE_SIZE.x, 0.0f));}
		else if (name == "Debug"){mDebugGrid = !mDebugGrid;}
		else if (name == "Quit"){
			ss.Serialize(this, "level01.level");
			mGame->Quit();
		}
		else if (name == "Load")
		{
			ss.Deserialize(this, "level01.level");
		}
		
		if (name == "LeftClick"){
			for (auto e : mEnities.getEntities())
				if (e->hasComponent<CDraggable>())
				{
					if (point_inside_entity(vec2(mMouseCursor.getPosition().x, mMouseCursor.getPosition().y), e))
					{
						e->getComponent<CDraggable>().dragging = true;
						mSelectedEntity = e;
						break;
					}

				}
		}
		//check if entity is clicked
		if (name == "RightClick" || name == "MiddleClick")
		{
			for (auto e : mEnities.getEntities()){
					if (point_inside_entity(vec2(mMouseCursor.getPosition().x, mMouseCursor.getPosition().y), e))
					{
						mSelectedEntity = e;
						break;
					}
				}
		}
	}
	else if (type == "END")
	{
		if (name == "LeftClick")
		{
			//set the dragging to false for all entities with Dragable component
			for (auto e : mEnities.getEntities())
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
		PlaceEntityBasedOnMouse (action.Position());
	}
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
	
		auto width = (mGame->Window().getSize().x);
		auto height = (mGame->Window().getSize().y);
		auto view = mGame->Window().getView();
		float leftX = view.getCenter().x - width / 2;
		float rightX = leftX + width+ TILE_SIZE.x;
		float topY = mGame->Window().getView().getCenter().y - height / 2;
		float bottomY = topY + height + TILE_SIZE.y;
		for (int i = 0; i < width; i++)
		{
			drawLine(vec2(leftX + i * TILE_SIZE.x, topY), vec2(leftX + i * TILE_SIZE.x, bottomY), mGame->Window());
		}
		for (int i = 0; i < height; i++)
		{
			drawLine(vec2(leftX, topY + i * TILE_SIZE.y), vec2(rightX, topY + i * TILE_SIZE.y), mGame->Window());
		}
	
}


void Editor::DrawSelectableTexture()
{
	auto pos = mGame->Window().mapPixelToCoords(sf::Mouse::getPosition(mGame->Window()));
	//start imgui window
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
		// place the text with of the image
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
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("%s", e.c_str());
			ImGui::EndTooltip();
		}
		
	}
	// put the name of the texture in the window
	ImGui::End();
}

void Editor::CreateSelectedEntity(const std::string& name, const vec2& pos)
{
	
	auto animation = mGame->GetAsset().getAnimation(name);
	auto entity = mEnities.addEntity(name);
	auto x = (int)(pos.x / TILE_SIZE.x) * TILE_SIZE.x;
	auto y = (int)(pos.y / TILE_SIZE.y) * TILE_SIZE.y;
	//this 3 cause I think it is essential  to the level editor
	entity->addComponent<CTransform>(vec2(x,y), vec2(1,1), 0.f);
	entity->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
	entity->addComponent<CDraggable>();
	entity->getComponent< CDraggable>().dragging = true;
	mSelectedEntity = entity;
}

void Editor::PlaceEntityBasedOnMouse(const vec2& pos)
{
	const auto mouse_pos = sf::Vector2i(pos.x, pos.y);

	//update the mouse cursor position based on the view
	const auto delta = mGame->Window().mapPixelToCoords(mouse_pos) - mMouseCursor.getPosition();
	//set the mouse cursor position to the new position
	mMouseCursor.setPosition(mMouseCursor.getPosition() + delta);

	// snap to the grid to the top right coner. + TIle/size.x/2 because the center is in middle
	auto x = ((int)(pos.x / TILE_SIZE.x) * TILE_SIZE.x) + TILE_SIZE.x / 2;
	auto y = ((int)(pos.y / TILE_SIZE.y) * TILE_SIZE.y) + TILE_SIZE.y / 2;
	//update x, y based on the view
	x = mGame->Window().mapPixelToCoords(sf::Vector2i(x, y)).x;
	y = mGame->Window().mapPixelToCoords(sf::Vector2i(x, y)).y;

	for (auto e : mEnities.getEntities())
	{
		if (e->hasComponent<CDraggable>())
		{
			if (e->getComponent<CDraggable>().dragging)
			{
				e->getComponent<CTransform>().position = vec2(x, y);
			}
		}
	}
}

void Editor::PropertyWindow(std::shared_ptr<Entity> e)
{
	
	if (ImGui::CollapsingHeader("Transform")) {

		if (e->hasComponent<CTransform>())
		{
			DrawVec2Control ("Position", e->getComponent<CTransform>().position);
			DrawVec2Control ("Scale", e->getComponent<CTransform>().scale, vec2(1, 1));
			DrawFloatDControl("Rotation", e->getComponent<CTransform>().angle);
		}
	}
	if (ImGui::CollapsingHeader("Animation")) {
		if (e->hasComponent<CAnimation>())
		{
			auto& animation = e->getComponent<CAnimation>().animation;
			auto animation_name = animation.GetName();
			if (ImGui::InputText("Name", &animation_name))
			{
				animation = mGame->GetAsset().getAnimation(animation_name);
			}
			ImGui::Text("Frame Count: %d", (int)animation.GetFrameCount());
		}
		else {
			std::string name{};
			ImGui::InputText("Name", &name[0], name.size());
			auto animation = mGame->GetAsset().getAnimation(name);
			e->addComponent<CAnimation>(animation, true);
		}
	}
	if (ImGui::CollapsingHeader("Draggable")) {
		//check box for dragable component
		if (e->hasComponent<CDraggable>())
		{
			ImGui::Checkbox("Dragging", &e->getComponent<CDraggable>().dragging);
		}
	}
	if (ImGui::CollapsingHeader("Health")) {
		if (e->hasComponent<CHealth>())
		{
			ImGui::InputInt("Health", &e->getComponent<CHealth>().health);
		}
		else {
			int health = 0;
			ImGui::InputInt("Health Level", &health);
			e->addComponent<CHealth>(health);
		}
	}
	if (ImGui::CollapsingHeader("Bounding Box")) {
		if (e->hasComponent<CBoundingBox>())
		{
			auto size = e->getComponent<CBoundingBox>().size;
			ImGui::Text("Width: %f, Height: %f", size.x, size.y);
		}
		else {
			//check box to add bounding box
			bool boundingBox = false;
			ImGui::Checkbox("Check to add Bounding Box", &boundingBox);
			if (boundingBox)
			{
				e->addComponent<CBoundingBox>(
					e->getComponent<CAnimation>().animation.GetSize());
			}
		}
	}
	if( ImGui::Button("Delete") )
	{
		e->destroy();
	}
}
void Editor::DrawVec2Control(const std::string& label, vec2& values, const vec2& reset_value, float default_width)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, default_width);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	float line_height = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.f;
	auto button_size = ImVec2(line_height + 3.f, line_height);
	ImGui::PushStyleColor (ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor (ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.f, 1.0f));
	ImGui::PushStyleColor (ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	if (ImGui::Button("X", button_size)) { values = reset_value; }
	ImGui::SameLine();
	ImGui::DragFloat("##X",&values.x, 0.1f);
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	ImGui::PopItemWidth();
	ImGui::PushStyleColor (ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor (ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.5f, 1.0f));
	ImGui::PushStyleColor (ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	
	if (ImGui::Button("Y", button_size)) { values.y = 0; }
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f);
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);
	ImGui::Columns(1);
	ImGui::PopID();
}
void Editor::DrawFloatDControl(const std::string& label, float& value, float reset_value, float default_width)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, default_width);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	if (ImGui::Button("Value")) { value = reset_value; }
	ImGui::SameLine();
	ImGui::DragFloat("##Value", &value, 0.1f);
	ImGui::SameLine();
	ImGui::PopItemWidth();
	ImGui::Columns(1);
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	
}
//make it easy to see the selected entity/texture
void Editor::DrawSelectedEntityBoarder()
{
	auto& t = mSelectedEntity->getComponent<CTransform>();
	auto size = mSelectedEntity->getComponent<CAnimation>().animation.GetSize();
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(size.x, size.y));
	rect.setPosition(t.position.x, t.position.y);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(2.0f);
	rect.setOrigin({ TILE_SIZE.x / 2, TILE_SIZE.y / 2 });
	rect.setRotation( t.angle );
	rect.setScale( t.scale.x, t.scale.y );
	mGame->Window().draw(rect);
}

void Editor::MoveScreenView(const vec2& dir)
{
	auto view = mGame->Window().getView();
	view.move(dir.x, dir.y);
	mGame->Window().setView(view);
}
