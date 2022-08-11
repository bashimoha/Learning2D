#pragma once
//simple animation:(
#include "Common.h"
class Animation
{
public:
	Animation() = default;
	Animation(const std::string& name, const sf::Texture& texture);
	Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, float speed);
	void Update();
	void SetFrame(size_t frame);
	bool IsDone() const;
	std::string GetName();
	vec2 GetSize() const;
	sf::Sprite& GetSprite();
	size_t GetFrameCount() const;
	
	
private:
	std::string mName;
	vec2 mSize{};
	size_t mCurrentFrame{0};
	size_t mTotalFrames{0};
	size_t mSpeed{ 0 };
	bool mDone{ false };
	sf::Sprite mSprite;
};

