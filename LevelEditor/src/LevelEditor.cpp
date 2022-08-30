#include "LevelEditor.h"
#include <Windows.h>
#include <commdlg.h>



static void drawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, sf::RenderTarget* target)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};
	//make each line a red line
	line[0].color = sf::Color::White;
	line[1].color = sf::Color::White;
	target->draw(line, 2, sf::Lines);
};




Editor::Editor(Engine* engine)
	:Scene(engine)
{
	registerAction(sf::Keyboard::Escape, "Quit");
	registerAction(sf::Keyboard::G, "Debug");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Q, "ZOOM_IN");
	registerAction(sf::Keyboard::E, "ZOOM_OUT");
	registerAction(sf::Keyboard::P, "Play");
	registerAction(sf::Keyboard::N, "Create");

	mSerializer = SceneSerializer(this);
	Init();
}

void Editor::Init()
{
	std::string title{ "Level Editor" };
	
	auto window_size = mGame->Window().getSize();
	mGame->Window().create(sf::VideoMode(window_size.x, window_size.y), title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	mGame->Window().setTitle(title);

	mMouseCursor.setRadius(10.0f);
	mMouseCursor.setPointCount(40);
	mMouseCursor.setFillColor(sf::Color::Red);
	mMouseCursor.setOrigin(mMouseCursor.getRadius(), mMouseCursor.getRadius());
	auto pos = sf::Mouse::getPosition();
	mMouseCursor.setPosition(sf::Vector2f(pos.x, pos.y));
	mMouseCursor.setOutlineColor(sf::Color::Black);

	for (size_t i = 0; i < BACKGROUNDLAYERS; i++)
	{
		auto layer_name = "background"+ std::to_string(i);
		mBackgroundLayer[i].setTexture(mGame->GetAsset().getTexture(layer_name));
		mBackgroundLayer[i].setScale(window_size.x / mBackgroundLayer[i].getLocalBounds().width, window_size.y / mBackgroundLayer[i].getLocalBounds().height);
		//pos
		mBackgroundLayer[i].setPosition(0, 0);
	}
}

void Editor::Update()
{
	mEntities.update();
	sAnimation();
	ImGui::SFML::Update(mGame->Window(), deltaClock.restart());

	
}
void Editor::sAnimation()
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
void Editor::drawBg()
{
	scroll = 0;
	//infinite scrolling background
	for (int x = 0; x < 10; x++)
	{
		auto speed = scroll;
		for(int i = 0; i < BACKGROUNDLAYERS; i++)
		{
			auto pos = mBackgroundLayer[i].getLocalBounds().width;
			pos *= mBackgroundLayer[i].getScale().x;
			mBackgroundLayer[i].setPosition(sf::Vector2f((pos * x)-speed, 0));
			//give it illusin of scrolling
			mGame->Window().draw(mBackgroundLayer[i]);
			speed+=10.f;
		}
		scroll += 0.2f;
	}
}
void Editor::Render()
{
	mGame->Window().clear();
	drawBg();

	if (mDebugGrid)
	{
		DrawGrid();
	}
	if (mSelectedEntity)
	{
		DrawSelectedEntityBoarder();
	}
	for (auto e : mEntities.getEntities())
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
		if (e->hasComponent<CRenderable>())
		{
			auto& entity = e->getComponent<CRenderable>();
			entity.shape.setRotation(t.angle);
			entity.shape.setPosition(t.position.x, t.position.y);
			entity.shape.setScale(t.scale.x, t.scale.y);
			mGame->Window().draw(entity.shape);
		}
	}
	//mRenderTexture.display();
	//auto sprite = sf::Sprite(mRenderTexture.getTexture());
	//mGame->Window().draw(sprite);
	mGame->Window().draw(mMouseCursor);
	

	_imgui();


}

void Editor::DoAction(const Action& action)
{
	auto name = action.Name();
	auto type = action.Type();
	auto velocity = vec2(0.0f, 0.0f);
	auto fElapsedTime = deltaClock.getElapsedTime();
	deltaClock.restart();
	const auto SPEED = 64;
	if (type == "BEGIN")
	{
		// wasd movement to move the view
		if (name == "ZOOM_IN")
		{
			ZoomIN();
		}
		else if (name == "ZOOM_OUT")
		{
			ZoomOut();
		}
		else if (name == "Create") {
			CreateNENTT( 1000);
		}
		else if (name == "UP")
		{
			velocity.y = -SPEED;
			std::cout << velocity.y << std::endl;
		}
		else if (name == "DOWN")
		{
			velocity.y = SPEED;
			mBackgroundLayer[0].move(5, 0);
		}
		else if (name == "LEFT")
		{
			velocity.x = -SPEED;
		}
		else if (name == "RIGHT") {
			velocity.x = SPEED;
		}	
		else if (name == "Quit"){
			mGame->Quit();
		}
		else if (name == "Play")
		{
			mGame->ChangeScene("Play", std::make_shared<PlayGame>(mGame,  this));
		}
		else if (name == "Debug") {
			mDebugGrid = !mDebugGrid;
		}
		else if (name == "LeftClick"){
			for (auto e : mEntities.getEntities())
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
			for (auto e : mEntities.getEntities()){
					if (point_inside_entity(vec2(mMouseCursor.getPosition().x, mMouseCursor.getPosition().y), e))
					{
						mSelectedEntity = e;
						break;
					}
				}
		}
		MoveScreenView(velocity);
	}
	else if (type == "END")
	{
		if (name == "LeftClick")
		{
			//set the dragging to false for all entities with Dragable component
			for (auto e : mEntities.getEntities())
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
	
		auto width = (mGame->Window().getSize().x);
		auto height = (mGame->Window().getSize().y);
		auto view = mGame->Window().getView();
		float leftX = view.getCenter().x - width / 2;
		float rightX = leftX + width+ TILE_SIZE.x;
		float topY = mGame->Window().getView().getCenter().y - height / 2;
		float bottomY = topY + height + TILE_SIZE.y;
		for (int i = 0; i < width; i++)
		{
			drawLine({ leftX + i * TILE_SIZE.x, topY }, { leftX + i * TILE_SIZE.x, bottomY }, &mRenderTexture);
		}
		for (int i = 0; i < height; i++)
		{
			drawLine({ leftX, topY + i * TILE_SIZE.y }, { rightX, topY + i * TILE_SIZE.y }, &mRenderTexture);
		}
}


void Editor::DrawSelectableTexture()
{
	auto pos = mGame->Window().mapPixelToCoords(sf::Mouse::getPosition(mGame->Window()));
	ImGui::Begin("Choose a Texture");
	ImGui::Text("Mouse Pos (x: %f), (y: %f)", pos.x, pos.y);
	int same_line = 0;
	auto size = sf::Vector2f(64, 64);
	int padding = 10;
	int item_per_line = 5;
	size_t total_item = mGame->GetAsset().getAnimationNames().size();
	size_t current = 0;
	//make a button that creates n number of entties when clicked
	
	for (auto e : mGame->GetAsset().getAnimationNames())
	{
		// the char _ is just for creating texture from Atlas and later should be used in ANimation
		if (!e.contains("_"))
		{
			++current;

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
				CreateSelectedEntity(e, vec2(pos.x, pos.y));
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("%s", e.c_str());
				ImGui::EndTooltip();
			}
			if (current == total_item)
			{
				//draw Prmitive sfml shapes
				ImGui::NewLine();
				ImGui::Text("Primitives");
				ImGui::Separator();
				ImGui::Button("Rectangle");
				if (ImGui::IsItemClicked())
				{
					CreateSelectedEntity("Rectangle", vec2(pos.x, pos.y), true);
				}
				ImGui::SameLine();
				ImGui::Button("Circle");
				if (ImGui::IsItemClicked())
				{
					CreateSelectedEntity("Circle", vec2(pos.x, pos.y), true);
				}
				ImGui::SameLine();
				ImGui::Button("Line");
				if (ImGui::IsItemClicked())
				{
					CreateSelectedEntity("Line", vec2(pos.x, pos.y), true);
				}
				ImGui::SameLine();
				ImGui::Button("Point");
				if (ImGui::IsItemClicked())
				{
					CreateSelectedEntity("Point", vec2(pos.x, pos.y), true);
				}
				ImGui::SameLine();
				ImGui::Button("Triangle");
				if (ImGui::IsItemClicked())
				{
					CreateSelectedEntity("Triangle", vec2(pos.x, pos.y), true);
				}

			}
		}
	}
	// put the name of the texture in the window
	ImGui::End();
}

void Editor::draw_sfml_primitive(const std::string& name, const vec2& pos)
{
	auto x = (int)(pos.x / TILE_SIZE.x) * TILE_SIZE.x;
	auto y = (int)(pos.y / TILE_SIZE.y) * TILE_SIZE.y;
	auto e = mEntities.addEntity(name);
	auto bouding_box_size = vec2(TILE_SIZE.x/2, TILE_SIZE.y/2);
	auto& tc = e->addComponent<CTransform>(vec2(x, y), vec2(1, 1), 0.f);
	e->addComponent<CDraggable>();
	if (name == "Rectangle")
	{
		auto& s = e->addComponent<CRenderable>( TILE_SIZE.x/2, 4, sf::Color::White, sf::Color::Black, 1.0f);
		e->addComponent <CBoundingBox>(bouding_box_size);
	   tc.angle = (45.f);
	}
	else if (name == "Circle")
	{
		e->addComponent <CRenderable>( TILE_SIZE.x/2, 32, sf::Color::White, sf::Color::Black, 1.0f);
		e->addComponent <CBoundingBox>(bouding_box_size);
	}
	else if (name == "Line")
	{
		e->addComponent <CRenderable>( TILE_SIZE.x/2, 2, sf::Color::White, sf::Color::Black, 1.0f);
		e->addComponent <CBoundingBox>(bouding_box_size);
	}
	else if (name == "Point")
	{
		e->addComponent <CRenderable>( TILE_SIZE.x/2, 2, sf::Color::White, sf::Color::Black, 1.0f);
		e->addComponent <CBoundingBox>(bouding_box_size);
	}
	else if (name == "Triangle")
	{
		e->addComponent <CRenderable>( TILE_SIZE.x/2, 3, sf::Color::White, sf::Color::Black, 1.0f);
		e->addComponent <CBoundingBox>(bouding_box_size);
	}
}

void Editor::CreateNENTT(int count)
{
	auto mSelectedEntity_pos = mSelectedEntity->getComponent < CTransform >().position;
	auto mSelectedEntity_size = mSelectedEntity->getComponent < CBoundingBox >();
	float posx = mSelectedEntity_pos.x + mSelectedEntity_size.size.x/2;
	float posy = mSelectedEntity_pos.y;
	
	for (int i = 0; i < count; i++)
	{
		auto e = mEntities.addEntity(mSelectedEntity->Tag());
		if (mSelectedEntity->hasComponent<CAnimation>())
		{
			auto& anim = e->addComponent<CAnimation>(mSelectedEntity->getComponent<CAnimation>());
		}
		if (mSelectedEntity->hasComponent<CRenderable>())
		{
			auto& render = e->addComponent<CRenderable>(mSelectedEntity->getComponent<CRenderable>());
		}
		if (mSelectedEntity->hasComponent<CDraggable>())
		{
			auto& draggable = e->addComponent<CDraggable>(mSelectedEntity->getComponent<CDraggable>());
		}
		auto& transform = e->addComponent<CTransform>(vec2(posx, posy), vec2(1, 1), 0.f);
		auto& bounding_box = e->addComponent<CBoundingBox>(mSelectedEntity->getComponent<CBoundingBox>());
		posx += mSelectedEntity_size.size.x/2;
		if (i == count - 1)
		{
			mSelectedEntity = e;
		}
	}
}

void Editor::CreateSelectedEntity(const std::string& name, const vec2& pos, bool primitive)
{
	if (primitive) {
		draw_sfml_primitive (name, pos);
	}
	else {
		auto animation = mGame->GetAsset().getAnimation(name);
		auto entity = mEntities.addEntity(name);
		auto x = (int)(pos.x / TILE_SIZE.x) * TILE_SIZE.x;
		auto y = (int)(pos.y / TILE_SIZE.y) * TILE_SIZE.y;
		//this 3 cause I think it is essential  to the level editor
		auto tc = entity->addComponent<CTransform>(vec2(x, y), vec2(1, 1), 0.f);
		auto ac = entity->addComponent<CAnimation>(mGame->GetAsset().getAnimation(name), true);
		entity->addComponent<CDraggable>();
		entity->getComponent< CDraggable>().dragging = true;
		auto anime_size = ac.animation.GetSize();
		auto bbx = anime_size.x * tc.scale.x;
		auto bby = anime_size.y * tc.scale.y;
		auto bbc = entity->addComponent<CBoundingBox>(
			vec2(bbx, bby));
		mSelectedEntity = entity;
	}
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

	for (auto e : mEntities.getEntities())
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
	//start imgui window
	
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
			//update the bounding box size to match the scaled 
			auto anime_size = e->getComponent<CAnimation>().animation.GetSize();
			auto x = anime_size.x * e->getComponent<CTransform>().scale.x;
			auto y = anime_size.y * e->getComponent<CTransform>().scale.y;
			auto& bbc/*hmmm*/  = e->getComponent<CBoundingBox>();
			bbc.size = vec2(x, y);
			ImGui::Text("Width: %f, Height: %f", bbc.size.x, bbc.size.y);
			//check box to add collider
			bool boundingBox = false;
			ImGui::Checkbox("Check to add Bounding Box", &boundingBox);
			if (boundingBox)
			{
				//apply the scale to make new bounding box accounitng for the new size
				bbc.collidable = true;
			}
		}
	}
	if( ImGui::Button("Delete") )
	{
		e->destroy();
		mSelectedEntity = nullptr;
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
std::string Editor::OpenFileDialog(const char* filter) noexcept
{
	auto window_handle = mGame->Window().getSystemHandle();
	char path[260] = {0};
	OPENFILENAMEA ofn{};
	ZeroMemory (&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.hwndOwner = window_handle;
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&ofn))
	{
		return path;
	}
	return  "";
	
}
std::string Editor::SaveFileDialog(const char* filter) noexcept
{
	auto window_handle = mGame->Window().getSystemHandle();
	char path[260] = {0};
	OPENFILENAMEA ofn{};
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.hwndOwner = window_handle;
	ofn.nMaxFile = sizeof(path);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileNameA(&ofn))
	{
		return path;
	}
	return "";
}

void Editor::FileDialogUI()
{
	ImGui::Begin("File##");
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
		}
		if (ImGui::MenuItem("Open"))
		{
			auto path = OpenFileDialog();
			if (!path.empty())
			{
				mEntities.removeAllEntities();
				mSerializer.Deserialize(path);
			}
		}
		if (ImGui::MenuItem("Save"))
		{
			mSerializer.Serialize(mLastOpenedFile);
		}
		if (ImGui::MenuItem("Save As"))
		{
			auto path = SaveFileDialog();
			if (!path.empty())
			{
				mSerializer.Serialize(path);
				mLastOpenedFile = path;
			}
		}
		ImGui::EndMenu();
	}
	ImGui::End();
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
	auto& t =mSelectedEntity ->getComponent<CTransform>();
	auto bb = mSelectedEntity->getComponent<CBoundingBox>().size;
	sf::RectangleShape rect;
	rect.setPosition(t.position.x, t.position.y);
	rect.setSize(sf::Vector2f(bb.x, bb.y));
	rect.setOrigin(
		bb.x / 2,
		bb.y / 2);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(2.0f);
	//rect.setOrigin({ -TILE_SIZE.x / 2, -TILE_SIZE.y / 2 });
	rect.setRotation( t.angle );
	rect.setScale( t.scale.x, t.scale.y );
	mGame->Window().draw(rect);
}

void Editor::MoveScreenView(const vec2& dir)
{
	auto mMapSize = 6000;
	auto view = mGame->Window().getView();
	if (view.getCenter().x + dir.x < mMapSize- view.getSize().x / 2 && view.getCenter().x + dir.x > view.getSize().x / 2)
	{
		view.move(dir.x, 0);
		auto speed = 1;
		scroll = dir.x;
	}
	mGame->Window().setView(view);
	
}
void Editor::ZoomIN()
{
	auto view = mGame->Window().getView();
	view.zoom(mZoomFactor);
	mGame->Window().setView(view);
}
void Editor::ZoomOut()
{
	auto view = mGame->Window().getView();
	view.zoom(1/ mZoomFactor);
	mGame->Window().setView(view);
}
void Editor::_imgui()
{
	ImGui::Begin("Create Entt");
	ImGui::Text("How many entities do you want to create?");
	DrawFloatDControl("Number of Entities", nEntt);
	if (ImGui::Button("Create"))
	{
		CreateNENTT(nEntt);

	}
	ImGui::End();
	DrawSelectableTexture();
	ImGui::Begin("Properties");
	if (mSelectedEntity)
	{
		PropertyWindow(mSelectedEntity);
	}
	FileDialogUI();
	ImGui::End();
	ImGui::SFML::Render(mGame->Window());
}



//////////////////////Play Game Class starts here/////////////////////////////////////
#pragma region PlayGame

PlayGame::PlayGame(Engine* engine, Editor* editor, const std::string& scene_name)
	:Scene(engine)
{
	mSceneName = scene_name;
	mEditor = editor;
	Init();
}
void PlayGame::Init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "Down");
	registerAction(sf::Keyboard::A, "Left");
	registerAction(sf::Keyboard::D, "Right");
	registerAction(sf::Keyboard::Escape, "Quit");
	
	mEntities.removeAllEntities();
	for( auto& e : mEditor->mEntities.getEntities())
	{
		auto entity = mEntities.addEntity(e->Tag());
		if(e->hasComponent<CTransform>())
		{ entity->addComponent<CTransform>(e->getComponent<CTransform>());}
		if(e->hasComponent<CGravity>())
		{ entity->addComponent<CGravity>(e->getComponent<CGravity>());}
		if(e->hasComponent<CHealth>())
		{ entity->addComponent<CHealth>(e->getComponent<CHealth>());}
		if(e->hasComponent<CInput>())
		{ entity->addComponent<CInput>(e->getComponent<CInput>());}
		if(e->hasComponent<CBoundingBox>())
		{ entity->addComponent<CBoundingBox>(e->getComponent<CBoundingBox>());}
		if(e->hasComponent<CState>())
		{ entity->addComponent<CState>(e->getComponent<CState>());}
		if(e->hasComponent<CAnimation>())
		{ entity->addComponent<CAnimation>(e->getComponent<CAnimation>());}
		auto anim = entity->getComponent<CAnimation>();
		if (anim.animation.GetName() == "player")
		{
			std::cout << anim.animation.GetName() << std::endl;
			mPlayer = entity;
			//attach input to player
			 mPlayer->addComponent<CInput>();
		}
	}
	
}
void PlayGame::Update()
{
	mEntities.update();
	
	//move mPlayer
	auto elpasedTime = deltaClock.getElapsedTime();
	deltaClock.restart();
	#if 0
	try
	{
		auto input = mPlayer->getComponent<CInput>();
		auto& tc = mPlayer->getComponent<CTransform>();
		vec2 move = { 0,0 };
		//deltaClock
	
		if (input.up)
		{
			move.y -= 64;
		}
		if (input.down)
		{
			move.y += 64;
		}
		if (input.left)
		{
			move.x -= 64;
		}
		if (input.right)
		{
			move.x += 64;
		}
		move *= elpasedTime.asSeconds();
		tc.velocity = move;
		tc.position += tc.velocity;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	#endif
}
void PlayGame::Render()
{
	std::cout << " I am in PlayGame" << std::endl;
	for (auto e : mEntities.getEntities())
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
void PlayGame::DoAction(const Action& action)
{
	//auto type = action.Type();
	//auto name = action.Name();
	//if (type == "BEGIN")
	//{
	//	if (name == "UP") {
	//		mPlayer->getComponent<CInput>().up = true;
	//	}
	//	else if (name == "Right") {
	//		mPlayer->getComponent<CInput>().right = true;
	//	}
	//	else if (name == "Left") {
	//		mPlayer->getComponent<CInput>().left = true;
	//	}
	//	else if (name == "Down") {
	//		mPlayer->getComponent<CInput>().down = true;
	//	}
	//}
	//if (type == "END")
	//{
	//	if (name == "UP") {
	//		mPlayer->getComponent<CInput>().up = false;
	//	}
	//	else if (name == "Right") {
	//		mPlayer->getComponent<CInput>().right = false;
	//	}
	//	else if (name == "Left") {
	//		mPlayer->getComponent<CInput>().left = false;
	//	}
	//	else if (name == "Down") {
	//		mPlayer->getComponent<CInput>().down = false;
	//	}
	//	else if (name == "Quit") {
	//		mGame->ChangeScene(mSceneName);
	//	}
	//}
}
#pragma endregion