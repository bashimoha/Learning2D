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
	mAnimations[name] = Animation(name, texture, frameCount, speed);
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
	return mAnimations[name];
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


void Assets::addTextureAtlas(const std::string& name, const std::string& path)
{
	sf::Image image;
	try
	{
		image.loadFromFile(path);
	}
	catch (const std::exception& e)
	{
		std::cout << "Failed to load texture atlas: " << path << std::endl;
		return;
	}
	mTextureAtlases[name] = image;
	std::cout << "Loaded texture atlas: " << path << std::endl;
}
void Assets::addTexture(const std::string& name, const std::string& texture_atlas, const vec2& pos, const vec2& size)
{
	try
	{
		sf::Texture texture;
		auto atlas = mTextureAtlases.at(texture_atlas);
		sf::IntRect rect(pos.x, pos.y, size.x, size.y);
		texture.loadFromImage(atlas, rect);
		mTextures[name] = texture;
	}
	catch (const std::exception& e)
	{
		std::cout << "Failed to load texture: " << name << std::endl;
		return;
	}
	mTextureNames.push_back(name);
	std::cout << "Loaded texture: " << name << std::endl;
	
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
			std::string name,tex_type, texture_name;
			int frame{ 0 }, frame_count{0};
			file >> name >> tex_type >> texture_name;
			if (tex_type == "TexAtlas")
			{
				int x, y, w, h; 
				file >> x >> y >> w >> h;
				addTexture (texture_name+name, texture_name, vec2(x, y), vec2(w, h));
				texture_name = texture_name + name;
				frame = 1;
				frame_count = 0;
			}
			else
			{
				file >> frame >> frame_count;
			}
			addAnimation(name, getTexture(texture_name), frame, frame_count);
		}
		else if (type == "TexAtlas")
		{
			std::string name, path;
			file >> name >> path;
			std::cout << "Loaded Texture Atlas: " << path << std::endl;
			addTextureAtlas(name, path);
		}
		else
		{
			std::cout << "Unknown asset type: " << type << std::endl;
		}
	}
}
