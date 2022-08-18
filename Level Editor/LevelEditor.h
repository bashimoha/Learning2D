#include "2D.h"
#include "imgui.h"
#include "imgui-SFML.h"
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
	sf::Clock deltaClock{};
	sf::CircleShape shape{};
	vec2 shape_input_pos{};
	const vec2 TILE_SIZE{ 64, 64 };
	EntityManager entityManager;
	sf::CircleShape mMouseCursor{};
	bool mDebugGrid{ false };
	std::string last_click{ "" };

	void DrawGrid();
	void DrawSelectableTexture();
	void CreateSelectedEntity(const std::string& name, const vec2& );
	
	
};