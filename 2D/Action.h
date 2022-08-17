#pragma once
#include "Common.h"
class Action
{
public:
	Action() = default;
	Action(const std::string& name, const std::string& type);
	Action(const std::string& name,  const vec2& pos);
	Action(const std::string& name, const std::string& type, const vec2& pos);
	std::string Name() const;
	std::string Type() const;
	vec2 Position() const;
	std::string toString() const;
	
private:
	std::string mName{"NONE"};
	std::string mType{ "NONE" };
	vec2 mPosition{ 0.0f, 0.0f };
};