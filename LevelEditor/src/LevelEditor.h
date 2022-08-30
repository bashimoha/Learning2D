//#include "2D.h"
#include "vec2.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include "SceneSerializer.h"

#include <SFML/System/Clock.hpp>
class PlayGame;
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
	void CreateSelectedEntity(const std::string& name, const vec2&, bool primitive = false);
	void PlaceEntityBasedOnMouse(const vec2& pos);
	void MoveScreenView(const vec2& dir);
	void PropertyWindow(std::shared_ptr<Entity> e);
	void DrawSelectedEntityBoarder();
	void DrawVec2Control(const std::string& label, vec2& values, const vec2& reset_value = vec2(0.f, 0.f), float default_width = 100.f);
	void DrawFloatDControl (const std::string& label, float& value, float reset_value = 0.f, float default_width = 100.f);
	//only .level files are supported
	std::string OpenFileDialog (const char* filter = "Level Files (*.level)\0*.level\0\0") noexcept;
	std::string SaveFileDialog (const char* filter = "Level Files (*.level)\0*.level\0\0") noexcept;
	void ZoomIN();
	void ZoomOut();
	void draw_sfml_primitive(const std::string& name, const vec2&);
	void CreateNENTT(int);
	void sAnimation();
	void drawBg();
private:
	float nEntt{ 0 };
	constexpr static size_t BACKGROUNDLAYERS = 3;
	std::array<sf::Sprite, BACKGROUNDLAYERS> mBackgroundLayer;
	float mZoomFactor = 1.5f;
	float scroll = 0;
	friend class PlayGame;
	sf::RenderTexture mRenderTexture;
	sf::View mRenderView;
	void FileDialogUI();
	sf::Clock deltaClock{};
	const vec2 TILE_SIZE{ 16, 16 };
	sf::CircleShape mMouseCursor{};
	bool mDebugGrid{ false };
	std::shared_ptr<Entity> mSelectedEntity{};
	SceneSerializer mSerializer{};
	std::string mLastOpenedFile = "";
	void _imgui();
	
};

class PlayGame : public Scene
{
public:
	PlayGame(Engine* engine,Editor* editor ,const std::string& scene_name = "LevelEditor");
	void Init();
	void Update();
	void Render();
	void  DoAction(const Action&);
private:
	sf::RenderTexture mRenderTexture;
	sf::Clock deltaClock{};
	SceneSerializer mSerializer{};
	std::string mSceneName{};
	Editor* mEditor = nullptr;
	std::shared_ptr<Entity> mPlayer;
};