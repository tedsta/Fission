#include "Fission/Core/ComponentTypeManager.h"

ComponentTypeManager::ComponentTypeManager()
{
    //ctor
}

ComponentType& ComponentTypeManager::getTypeFor(const std::type_info &t)
{
    ComponentType* type = mComponentTypes[t.hash_code()];

    if(type == nullptr) {
        type = new ComponentType();
        mComponentTypes[t.hash_code()] = type;
    }

    return *type;
}

void ComponentTypeManager::deleteComponentTypes()
{
    std::unordered_map<size_t,ComponentType*>::iterator it;

    for(it = mComponentTypes.begin(); it != mComponentTypes.end(); it++)
    {
        delete it->second;
    }
    mComponentTypes.clear();
}

std::unordered_map<size_t,ComponentType*>ComponentTypeManager::mComponentTypes;
