#include "MenuScene.h"
#include "Scene.h"
#include "Engine.h"
#include "GameScene.h"

Menu::Menu(Engine* engine)
	:Scene(engine)
{
	Init();
}

void Menu::Init()
{
	//register actions
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::P, "Play");
	registerAction(sf::Keyboard::Escape, "Quit");

	//menu_font = (mGame->GetAsset().getFont("Title"));
	mTitle = "Elite Jario {Menu}";
	auto window_size = mGame->Window().getSize();
	auto levels = 3;
	menu_font = mGame->GetAsset().getFont("LevelFont");
	
	mTitleText.setString("Jario");
	mTitleText.setFont(menu_font);
	mTitleText.setCharacterSize(100);
	mTitleText.setPosition(64, 0);
	mBackground.setSize(sf::Vector2f(window_size.x, window_size.y));
	
	for (int i = 0; i < levels; i++)
	{
		sf::Text lvl;
		lvl.setString("Level " + std::to_string(i+1));
		lvl.setFont(menu_font);
		lvl.setPosition(
			64, window_size.y / (levels + 1) * (i + 1)
		);
		lvl.setFillColor(sf::Color::White);
		lvl.setCharacterSize(64);
		mLevelNames.push_back(lvl);
	}
	mLevelPaths.push_back("level1.txt");
	mLevelPaths.push_back("level2.txt");
	mLevelPaths.push_back("level3.txt");
	
	mInstruction.setString("Play: P Quit:Esc  UP:W  Down:D");
	mInstruction.setFont(menu_font);
	mInstruction.setPosition(window_size.x - 62*8, window_size.y - 64);
	mInstruction.setFillColor(sf::Color::Black);
	mInstruction.setCharacterSize(32);
	mGame->Window().setTitle(mTitle);
}

void Menu::Update()
{
	mLevelNames[mIndex].setFillColor(sf::Color::Red);
}

void Menu::Render()
{
	mGame->Window().clear(sf::Color(0x015d85));
	mGame->Window().draw(mTitleText);
	mGame->Window().draw(mInstruction);
	int i = 0;
	for (auto lvl : mLevelNames)
	{
		if (i == mIndex)
		{
			auto l = lvl.getString();
			l = "{" + l + "}";
			lvl.setString(l);
		}
		i++;
		mGame->Window().draw(lvl);
	}
}

void Menu::DoAction(const Action& action)
{
	
	if (action.Type() == "BEGIN")
	{
		if (action.Name() == "UP")
		{
			mLevelNames[mIndex].setFillColor(sf::Color::White);
			if (mIndex > 0) { mIndex--; }
			else { mIndex = mLevelNames.size() - 1; }
		}
		else if (action.Name() == "DOWN")
		{
			mLevelNames[mIndex].setFillColor(sf::Color::White);
			if (mIndex < mLevelNames.size()-1) {mIndex++;}
		}
		else if (action.Name() == "Quit")
		{
			mGame->Quit();
		}
		else if (action.Name() == "Play")
		{
			mGame->ChangeScene("LevelScene", 
				std::make_shared<GameScene>(mGame, "assets/level01.txt"));
		}
	}
}
