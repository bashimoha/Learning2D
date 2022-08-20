#include "2D.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "SceneSerializer.h"

#include <SFML/System/Clock.hpp>

class Editor : public Scene
{
public:
	Editor(Engine* engine);
	void Init();
	void Update();
	void Render();
	void  DoAction(const Action&);
private:

	void DrawGrid();
	void DrawSelectableTexture();
	void CreateSelectedEntity(const std::string& name, const vec2& );
	void PlaceEntityBasedOnMouse(const vec2& pos);
	void MoveScreenView(const vec2& dir);
	void PropertyWindow(std::shared_ptr<Entity> e);
	void DrawSelectedEntityBoarder();
	void DrawVec2Control(const std::string& label, vec2& values, const vec2& reset_value = vec2(0.f, 0.f), float default_width = 100.f);
	void DrawFloatDControl (const std::string& label, float& value, float reset_value = 0.f, float default_width = 100.f);

	sf::Clock deltaClock{};
	const vec2 TILE_SIZE{ 64, 64 };
	sf::CircleShape mMouseCursor{};
	bool mDebugGrid{ false };
	std::shared_ptr<Entity> mSelectedEntity{};
	SceneSerializer ss{};
};