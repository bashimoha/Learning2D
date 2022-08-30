#include "Action.h"

Action::Action(const std::string& name, const std::string& type)
    :mName(name), mType(type)
{
}

Action::Action(const std::string& type, const vec2& pos)
    :mType(type), mPosition(pos)
{
}

Action::Action(const std::string& name, const std::string& type, const vec2& pos)
    : mName(name), mType(type), mPosition(pos)
{
}

vec2 Action::Position() const
{
	return mPosition;
}

std::string Action::Name() const
{
    return mName;
}

std::string Action::Type()const
{
    return mType;
}

std::string Action::toString()const
{
   return mName + ", " + mType;
}
