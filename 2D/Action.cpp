#include "Action.h"

Action::Action(const std::string& name, const std::string& type)
    :mName(name), mType(type)
{
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
