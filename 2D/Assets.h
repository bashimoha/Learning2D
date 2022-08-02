#pragma once
#include "Common.h"
#include "Animation.h"
class Assets
{
public:
	void LoadFromFile(const std::string& path);
	
	sf::Texture& getTexture(const std::string& name);
	sf::Font& getFont(const std::string& name);
	Animation& getAnimation(const std::string& name);
	
private:
	std::map<std::string, sf::Texture> mTextures{};
	std::map<std::string, Animation> mAnimation{};
	std::map<std::string, sf::Font> mFonts{};
	void addTexture(const std::string& name, const std::string& path, bool smooth);
	void addFont(const std::string& name, const std::string& path);
	void addAnimation(const std::string& name, const sf::Texture& texture, size_t frameCount, float speed);
};

