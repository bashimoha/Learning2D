#pragma once
#include "Common.h"
class Assets
{
public:
	void LoadFromFile(const std::string& path);
	void addTexture(const std::string& name, const std::string& path,bool smooth);
	void addFont(const std::string& name, const std::string& path);
	void addSound(const std::string& name, const std::string& path);
	
	sf::Texture& getTexture(const std::string& name);
	
private:
	std::map<std::string, sf::Texture> mTextures{};
	std::map<std::string, sf::Sound> mSounds{};
	std::map<std::string, sf::Font> mFonts{};
};

