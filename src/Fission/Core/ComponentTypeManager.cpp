#include "Fission/Core/ComponentTypeManager.h"

namespace fsn
{
    std::vector<std::bitset<MaxComponents>> ComponentTypeManager::mBits;
    std::vector<ComponentFactory> ComponentTypeManager::mFactories;
    int ComponentTypeManager::mNextID = 0;

    Component* ComponentTypeManager::createComponent(ComponentType type)
    {
        return mFactories[type]();
    }
}

