#pragma once
//simple animation:(
#include "Common.h"
class Animation
{
public:
	Animation() = default;
	Animation(const std::string& name, const sf::Texture& texture);
	Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, float speed);
	friend std::ostream& operator<<(std::ostream& os, const Animation& anim);
	void Update();
	void SetFrame(size_t frame);
	void SetName(const std::string&);
	bool IsDone() const;
	std::string GetName() const;
	vec2 GetSize() const;
	sf::Sprite& GetSprite();
	size_t GetFrameCount() const;
	
	
	vec2 mSize{};
private:
	std::string mName;
	size_t mCurrentFrame{0};
	size_t mTotalFrames{0};
	size_t mSpeed{ 0 };
	bool mDone{ false };
	sf::Sprite mSprite;
	
};

