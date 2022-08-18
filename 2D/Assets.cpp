#include "Assets.h"

void Assets::addTexture(const std::string& name, const std::string& path, bool smooth=false)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cout << "Failed to load texture: " << path << std::endl;
		return;
	}
	texture.setSmooth(smooth);
	mTextures[name] = texture;
	////////////////////////////
	mTextureNames.push_back(name);
	/////////////////////////////
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cout << "Could not load font: " << path << std::endl;
	}
	mFonts[name] = font;
	////////////////////////////
	mFontNames.push_back(name);
	/////////////////////////////
}

void Assets::addAnimation(const std::string& name, const sf::Texture& texture, size_t frameCount, float speed)
{
	mAnimation[name] = Animation(name, texture, frameCount, speed);
	////////////////////////////
	mAnimationNames.push_back(name);
	/////////////////////////////
}

sf::Texture& Assets::getTexture(const std::string& name)
{
	return mTextures[name];
}

sf::Font& Assets::getFont(const std::string& name)
{
	//this is not safe cuz it will crash if the texture is not found
	return mFonts[name];
}

Animation& Assets::getAnimation(const std::string& name)
{
	return mAnimation[name];
}

const std::vector<std::string>& Assets::getTextureNames() const
{
	return mTextureNames;
}


const std::vector<std::string>& Assets::getFontNames() const
{
	return mFontNames;
}

const std::vector<std::string>& Assets::getAnimationNames() const
{
	return mAnimationNames;
}


void Assets::LoadFromFile(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "Could not open file: " << path << std::endl;
		return;
	}
	std::string type;
	while (file.good())
	{
		file >> type;
		//split the string for easy iteration
		if (type == "Texture")
		{
			std::string name, path;
			file >> name >> path;
			std::clog << "Loaded Texture: " << path << std::endl;
			addTexture(name, path);
		}
		else if(type == "Font")
		{
			std::string name, path;
			file >> name >> path;
			std::clog << "Loaded Font: " << path << std::endl;
			addFont(name, path);
		}
		else if(type == "Animation")
		{
			//sample animation format: Animation Stand64 TexStand64 1 0
			std::string name, texture_name;
			int frame, frame_count;
			file >> name >> texture_name>> frame >> frame_count;
			addAnimation(name, getTexture(texture_name), frame, frame_count);
		}
		else
		{
			std::cerr << "Unknown type: " << type << std::endl;
		}
	}
}
