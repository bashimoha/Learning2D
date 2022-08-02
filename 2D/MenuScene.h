#pragma once
#include "Common.h"
#include "Scene.h"
class Menu : public Scene
{
public:
	Menu(Engine* engine);
	void Init();
	void Update();
	void Render();
	void  DoAction(const Action&);
private:
	int mIndex{};
	std::vector<std::string> mLevelPaths{};
	std::vector<sf::Text> mLevelNames{};
	std::string mTitle{""};
	sf::Font menu_font;
	sf::Text mTitleText;
	sf::Text mInstruction;
	sf::RectangleShape mBackground;
};

