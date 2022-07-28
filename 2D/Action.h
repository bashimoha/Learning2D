#pragma once
#include "Common.h"
class Action
{
public:
	Action() = default;
	Action(const std::string& name, const std::string& type);
	std::string Name() const;
	std::string Type() const;
	std::string toString() const;
private:
	std::string mName{"NONE"};
	std::string mType{"NONE"};
};