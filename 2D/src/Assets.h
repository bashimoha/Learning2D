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
	const std::vector<std::string>& getTextureNames() const;
	const std::vector<std::string>& getFontNames() const;
	const std::vector<std::string>& getAnimationNames() const;
	
	
private:
	std::map<std::string, sf::Texture> mTextures{};
	std::map<std::string, Animation> mAnimations{};
	std::map<std::string, sf::Font> mFonts{};
	std::map<std::string, sf::Image> mTextureAtlases{};
	void addTexture(const std::string& name, const std::string& path, bool smooth);
	void addFont(const std::string& name, const std::string& path);
	void addAnimation(const std::string& name, const sf::Texture& texture, size_t frameCount, float speed);
	void addTexture(const std::string& name, const std::string& texture_atlas, const vec2& pos, const vec2& size);
	void addTextureAtlas(const std::string& name, const std::string& path);
	//not a good idea but works
	std::vector<std::string> mTextureNames;
	std::vector<std::string> mAnimationNames;
	std::vector<std::string> mFontNames;

};
