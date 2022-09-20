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
	mSerializer = new SceneSerializer(this);
	Init();
}
Editor::~Editor()
{
	delete mSerializer;
	delete mWorld;
	mSerializer = nullptr;
	mWorld = nullptr;
}
void Editor::Init()
{
	
	auto window_size = mEngine->Window().getSize();
	mEngine->Window().create(sf::VideoMode(window_size.x, window_size.y), mSceneName, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	mEngine->Window().setTitle(mSceneName);

	mMouseCursor.setRadius(10.0f);
	mMouseCursor.setPointCount(40);
	mMouseCursor.setFillColor(sf::Color::Red);
	mMouseCursor.setOrigin(mMouseCursor.getRadius(), mMouseCursor.getRadius());
	auto pos = sf::Mouse::getPosition();
	mMouseCursor.setPosition(sf::Vector2f((float)pos.x, (float)pos.y));
	mMouseCursor.setOutlineColor(sf::Color::Black);
	for (size_t i = 0; i < BACKGROUNDLAYERS; i++)
	{
		auto layer_name = "background"+ std::to_string(i);
		mBackgroundLayer[i].setTexture(mEngine->GetAsset().getTexture(layer_name));
		mBackgroundLayer[i].setScale(window_size.x / mBackgroundLayer[i].getLocalBounds().width, window_size.y / mBackgroundLayer[i].getLocalBounds().height);
		//pos
		mBackgroundLayer[i].setPosition(0, 0);
	}
	//setup physics world
	mWorld = new b2World(b2Vec2(0.0f, 9.8f));
	//populate the world with entities
	//create a ground
		//crete ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = mWorld->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	//it should take the whole width of the screen
	groundBox.SetAsBox(window_size.x, window_size.y-10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
}
void Editor::Update(sf::Clock deltaClock)
{
	mEntities.update();
	sAnimation();
	ImGui::SFML::Update(mEngine->Window(), deltaClock.restart());
	//step  physics world
	mWorld->Step(1.0f / 60.0f, 8, 3);
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
			mEngine->Window().draw(mBackgroundLayer[i]);
			speed+=10.f;
		}
		scroll += 0.2f;
	}
}
void Editor::Render()
{
	mEngine->Window().clear({ 97,83,109 });
	//drawBg();
	if (mDebugGrid)
	{
		DrawGrid(mEngine->Window());
	}
	if (mSelectedEntity)
	{
		DrawSelectedEntityBoarder();
	}
	for (auto e : mEntities.getEntities())
	{
		auto SCALE = 30.f;
		auto& t = e->getComponent<CTransform>();
		
		if (e->hasComponent<CAnimation>())
		{
			auto& entity = e->getComponent<CAnimation>();
			entity.animation.GetSprite().setRotation(t.angle);
			entity.animation.GetSprite().setPosition(t.position.x, t.position.y);
			entity.animation.GetSprite().setScale(t.scale.x, t.scale.y);
			mEngine->Window().draw(entity.animation.GetSprite());
		}
		else if( e->hasComponent<CRenderable>())
		{
			auto& entity = e->getComponent<CRenderable>();
			entity.shape.setPosition(t.position.x, t.position.y);
			entity.shape.setRotation(t.angle);
			entity.shape.setScale(t.scale.x, t.scale.y);
			//change size of shape using scale
			mEngine->Window().draw(entity.shape);
		}
	}
	mEngine->Window().draw(mMouseCursor);
	_imgui();
}
void Editor::DoAction(const Action& action)
{
	auto name = action.Name();
	auto type = action.Type();
	auto velocity = vec2(0.0f, 0.0f);
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
			delete mSerializer;
			mSerializer = nullptr;
			mEngine->Quit();
		}
		else if (name == "Play")
		{
			if (mLastOpenedFile == "")
			{
				mLastOpenedFile = "temp.level";
				
			}
			mSerializer->Serialize(mLastOpenedFile);
			mEngine->ChangeScene("Play", std::make_shared<PlayGame>(mEngine,  this));
		}
		else if (name == "Debug") {
			mDebugGrid = !mDebugGrid;
		}
		else if (name == "LeftClick"){
			for (auto e : mEntities.getEntities()){
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
void Editor::DrawGrid(sf::RenderWindow& target)
{
	
		auto width = (mEngine->Window().getSize().x);
		auto height = (mEngine->Window().getSize().y);
		auto view = mEngine->Window().getView();
		float leftX = view.getCenter().x - width / 2;
		float rightX = leftX + width+ TILE_SIZE.x;
		float topY = mEngine->Window().getView().getCenter().y - height / 2;
		float bottomY = topY + height + TILE_SIZE.y;
		for (int i = 0; i < width; i++)
		{
			drawLine({ leftX + i * TILE_SIZE.x, topY }, { leftX + i * TILE_SIZE.x, bottomY }, &target);
		}
		for (int i = 0; i < height; i++)
		{
			drawLine({ leftX, topY + i * TILE_SIZE.y }, { rightX, topY + i * TILE_SIZE.y }, &target);
		}
}
void Editor::DrawSelectableTexture()
{
	auto pos = mEngine->Window().mapPixelToCoords(sf::Mouse::getPosition(mEngine->Window()));
	ImGui::Begin("Choose a Texture");
	ImGui::Text("Mouse Pos (x: %f), (y: %f)", pos.x, pos.y);
	int same_line = 0;
	auto size = sf::Vector2f(64, 64);
	int padding = 10;
	int item_per_line = 5;
	size_t total_item = mEngine->GetAsset().getAnimationNames().size();
	size_t current = 0;
	//make a button that creates n number of entties when clicked
	
	for (auto e : mEngine->GetAsset().getAnimationNames())
	{
		// the char _ is just for creating texture from Atlas and later should be used in ANimation
		if (!e.contains("_"))
		{
			++current;

			ImGui::ImageButton(mEngine->GetAsset().getAnimation(e).GetSprite(),
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
		auto animation = mEngine->GetAsset().getAnimation(name);
		auto entity = mEntities.addEntity(name);
		auto x = (int)(pos.x / TILE_SIZE.x) * TILE_SIZE.x;
		auto y = (int)(pos.y / TILE_SIZE.y) * TILE_SIZE.y;
		//this 3 cause I think it is essential  to the level editor
		auto tc = entity->addComponent<CTransform>(vec2(x, y), vec2(1, 1), 0.f);
		auto ac = entity->addComponent<CAnimation>(mEngine->GetAsset().getAnimation(name), true);
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
	const auto delta = mEngine->Window().mapPixelToCoords(mouse_pos) - mMouseCursor.getPosition();
	//set the mouse cursor position to the new position
	mMouseCursor.setPosition(mMouseCursor.getPosition() + delta);

	// snap to the grid to the top right coner. + TIle/size.x/2 because the center is in middle
	auto x = ((int)(pos.x / TILE_SIZE.x) * TILE_SIZE.x) + TILE_SIZE.x / 2;
	auto y = ((int)(pos.y / TILE_SIZE.y) * TILE_SIZE.y) + TILE_SIZE.y / 2;
	//update x, y based on the view
	x = mEngine->Window().mapPixelToCoords(sf::Vector2i((int)x, (int)y)).x;
	y = mEngine->Window().mapPixelToCoords(sf::Vector2i((int)x, (int)y)).y;

	for (auto e : mEntities.getEntities())
	{
		if (e->hasComponent<CDraggable>())
		{
			if (e->getComponent<CDraggable>().dragging)
			{
				auto& p = e->getComponent<CTransform>();
				p.position= vec2(x, y);
				if(e->hasComponent<CRigidBody>())
					e->getComponent<CRigidBody>().body->SetTransform({ x / 30, y / 30 }, p.angle);
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
				animation = mEngine->GetAsset().getAnimation(animation_name);
			}
			ImGui::Text("Frame Count: %d", (int)animation.GetFrameCount());
		}
		else {
			std::string name{};
			ImGui::InputText("Name", &name[0], name.size());
			auto animation = mEngine->GetAsset().getAnimation(name);
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
			ImGui::Checkbox("Check to add Bounding Box", &bbc.collidable);
		}
	}
	//RigidBody
	if (ImGui::CollapsingHeader("RigidBody")) {
		if (e->hasComponent<CRigidBody>())
		{
			auto& rb = e->getComponent<CRigidBody>();
			auto body_type = rb.body->GetType();
			if (ImGui::RadioButton("Static", body_type == b2_staticBody))
			{
				rb.body->SetType(b2_staticBody);
			}
			if (ImGui::RadioButton("Dynamic", body_type == b2_dynamicBody))
			{
				rb.body->SetType(b2_dynamicBody);
			}
			// friction, density, restitution etc options
			float friction = rb.body->GetFixtureList()->GetFriction();
			float density = rb.body->GetFixtureList()->GetDensity();
			float restitution = rb.body->GetFixtureList()->GetRestitution();
			ImGui::SliderFloat("Friction", &friction, 0.0f, 1.0f);
			ImGui::SliderFloat("Density", &density, 0.0f, 1.0f);
			ImGui::SliderFloat("Restitution", &restitution, 0.0f, 1.0f);
			rb.fixture->SetFriction(friction);
			rb.fixture->SetDensity(density);
			rb.fixture->SetRestitution(restitution);
		}
		else {
			bool active = false;
			ImGui::Checkbox("Check to add RigidBody", &active);
			if (active)
			{
				auto& rb = e->addComponent<CRigidBody>();
				//create the body
				b2BodyDef body_def;
				//body type options
				if (ImGui::RadioButton("Static", true))
				{
					body_def.type = b2_staticBody;
				}
				if (ImGui::RadioButton("Dynamic", false))
				{
					body_def.type = b2_dynamicBody;
				}
				//set the position
				auto scale = 30.0f;
				body_def.position.Set(e->getComponent<CTransform>().position.x / scale, e->getComponent<CTransform>().position.y / scale);
				//set the angle
				body_def.angle = e->getComponent<CTransform>().angle;
				//add the body to the world and store the body pointer in the rigid body component
				rb.body = mWorld->CreateBody(&body_def);
				//create the fixture
				b2PolygonShape shape;
				//set the shape size
				auto anime_size = e->getComponent<CAnimation>().animation.GetSize();
				auto x = anime_size.x * e->getComponent<CTransform>().scale.x;
				auto y = anime_size.y * e->getComponent<CTransform>().scale.y;
				shape.SetAsBox(x / scale, y / scale);
				//create the fixture def
				b2FixtureDef fixture_def;
				fixture_def.shape = &shape;
				//fixture properties options
				ImGui::InputFloat("Friction", &fixture_def.friction);
				ImGui::InputFloat("Restitution", &fixture_def.restitution);
				ImGui::InputFloat("Density", &fixture_def.density);
				//add the fixture to the body
				rb.fixture = rb.body->CreateFixture(&fixture_def);
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
				mSerializer->Deserialize(path);
				mLastOpenedFile = path;
			}
		}
		if (ImGui::MenuItem("Save"))
		{
			mSerializer->Serialize(mLastOpenedFile);
		}
		if (ImGui::MenuItem("Save As"))
		{
			auto path = SaveFileDialog();
			if (!path.empty())
			{
				mSerializer->Serialize(path);
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
	mEngine->Window().draw(rect);
}
void Editor::MoveScreenView(const vec2& dir)
{
	auto mMapSize = 6000;
	auto view = mEngine->Window().getView();
	//only scroll if we are not at the edge of the map [ 0, MapSize ]
	if (view.getCenter().x + dir.x < mMapSize && view.getCenter().x + dir.x > mMapSize/10)
	{
		view.move(dir.x, dir.y);
	}
	mEngine->Window().setView(view);
	
}
void Editor::ZoomIN()
{
	auto view = mEngine->Window().getView();
	view.zoom(mZoomFactor);
	mEngine->Window().setView(view);
}
void Editor::ZoomOut()
{
	auto view = mEngine->Window().getView();
	view.zoom(1/ mZoomFactor);
	mEngine->Window().setView(view);
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
	ImGui::SFML::Render(mEngine->Window());
}
std::string Editor::OpenFileDialog(const char* filter) noexcept
{
	auto window_handle = mEngine->Window().getSystemHandle();
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
	auto window_handle = mEngine->Window().getSystemHandle();
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
//////////////////////Play Game Class starts here/////////////////////////////////////

#pragma region PlayGame
PlayGame::PlayGame(Engine* engine, Editor* editor, const std::string& scene_name)
	:Scene(engine)
{
	mSceneName = scene_name;
	mEditor = editor;
	Init();
}
PlayGame::~PlayGame()
{
	mEntities.removeAllEntities();
	delete mSerializer;
	delete mWorld;
	mSerializer = nullptr;
	mWorld = nullptr;
}
void PlayGame::Init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "Down");
	registerAction(sf::Keyboard::A, "Left");
	registerAction(sf::Keyboard::D, "Right");
	registerAction(sf::Keyboard::Escape, "Quit");

	mEntities.removeAllEntities();
	mSerializer = new SceneSerializer(this);
	mWorld = new b2World(b2Vec2(0.0f, 9.8f));
	mSerializer->Deserialize(mEditor->mLastOpenedFile);
}
void PlayGame::Update(sf::Clock deltaClock)
{
	mEntities.update();
	deltaClock.restart();
	mWorld->Step(1.0f / 60, 8, 3);
}
void PlayGame::Render()
{
	
	for (auto e : mEntities.getEntities())
	{
		auto SCALE = 30;
		auto& t = e->getComponent<CTransform>();
		if (e->hasComponent<CRigidBody>())
		{
			//extract the rigid body component
			auto& rigid_body = e->getComponent<CRigidBody>();
			//get the body position
			auto body_pos = rigid_body.body->GetPosition();
			//set the transform position to the body position
			t.position.x = body_pos.x * SCALE;
			t.position.y = body_pos.y * SCALE;
		}
		if (e->hasComponent<CAnimation>())
		{
			auto& entity = e->getComponent<CAnimation>();
			entity.animation.GetSprite().setRotation(t.angle);
			entity.animation.GetSprite().setPosition(t.position.x, t.position.y);
			entity.animation.GetSprite().setScale(t.scale.x, t.scale.y);

			mEngine->Window().draw(entity.animation.GetSprite());
		}
	}
}
void PlayGame::DoAction(const Action& action)
{
	auto type = action.Type();
	auto name = action.Name();
	if (type == "BEGIN")
	{
	}
	if (type == "END")
	{
		if (name == "Quit") {
			delete mSerializer;
			mSerializer = nullptr;
			mEngine->ChangeScene(mEditor->mSceneName);
		}
	}
}
#pragma endregion