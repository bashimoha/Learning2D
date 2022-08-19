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

	void DrawGrid();
	void DrawSelectableTexture();
	void CreateSelectedEntity(const std::string& name, const vec2& );
	void PlaceEntityBasedOnMouse(const vec2& pos);
	void MoveScreenView(const vec2& dir);

	
	sf::Clock deltaClock{};
	const vec2 TILE_SIZE{ 64, 64 };
	EntityManager entityManager;
	sf::CircleShape mMouseCursor{};
	bool mDebugGrid{ false };

};