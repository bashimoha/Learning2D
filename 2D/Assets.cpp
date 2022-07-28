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
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cout << "Could not load font: " << path << std::endl;
	}
	mFonts[name] = font;
}

void Assets::addSound(const std::string& name, const std::string& path)
{
		sf::SoundBuffer sound_buffer;
		if (!sound_buffer.loadFromFile(path))
		{
			std::cout << "Error loading sound file: " << path << std::endl;
		}
		else
		{
			sf::Sound sound;
			sound.setBuffer(sound_buffer);
			mSounds[name] = sound;
		}
}
sf::Texture& Assets::getTexture(const std::string& name)
{
	return mTextures[name];
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
			addTexture(name, path);
		}
		else if(type == "Font")
		{
			std::string name, path;
			file >> name >> path;
			addFont(name, path);
		}
		else if(type == "Sound")
		{
			std::string name, path;
			file >> name >> path;
			addSound(name, path);
		}
		else
		{
			std::cout << "Unknown type: " << type << std::endl;
		}
	}
}
