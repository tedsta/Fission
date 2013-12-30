#include "Fission/Core/ComponentTypeManager.h"

std::vector<std::bitset<MAX_COMPONENTS>> ComponentTypeManager::mBits;
std::vector<ComponentFactory> ComponentTypeManager::mFactories;
int ComponentTypeManager::mNextID = 0;
