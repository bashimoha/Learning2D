#include "Animation.h"



Animation::Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, float speed)
	:mName(name), mTotalFrames(frameCount), mSpeed(speed), mSprite(texture)
{
	mSize = vec2( (float)texture.getSize().x / frameCount, 
		          (float)texture.getSize().y
				);
	mSprite.setOrigin(mSize.x/2, mSize.y/2);
	//set the texture rect to the first frame
	mSprite.setTextureRect(sf::IntRect(std::floor(mCurrentFrame)*mSize.x, 0, mSize.x, mSize.y));
	
}
Animation::Animation(const std::string& name, const sf::Texture& texture)
	:Animation(name, texture, 1, 0)
{
}


void Animation::Update()
{
	mDone = ++mCurrentFrame > mTotalFrames;
	if (mDone)
	{
		
	}
	//TODO: obviously we gotta actually do something here like a actually update the sprite? 
}
bool Animation::IsDone() const
{
	return mDone;
}

std::string Animation::GetName()
{
	return mName;
}

vec2 Animation::GetSize() const
{
	return mSize;
}

sf::Sprite& Animation::GetSprite()
{
	return mSprite;
}
