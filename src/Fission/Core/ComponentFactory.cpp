#include "Fission/Core/ComponentFactory.h"

ComponentFactory::ComponentFactory()
{
    //ctor
}

ComponentType& ComponentFactory::getTypeFor(const std::type_info &t)
{
    ComponentType* type = mComponentTypes[t.hash_code()];

    if(type == nullptr) {
        type = new ComponentType();
        mComponentTypes[t.hash_code()] = type;
    }

    return *type;
}

void ComponentFactory::deleteComponentTypes()
{
    std::unordered_map<size_t,ComponentType*>::iterator it;

    for(it = mComponentTypes.begin(); it != mComponentTypes.end(); it++)
    {
        delete it->second;
    }
    mComponentTypes.clear();
}

std::unordered_map<size_t,ComponentType*>ComponentFactory::mComponentTypes;
