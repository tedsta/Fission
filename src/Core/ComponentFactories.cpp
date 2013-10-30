#include "Core/ComponentFactories.h"

std::vector<ComponentFactory> ComponentFactories::mComponentFactories;

TypeBits ComponentFactories::add(ComponentFactory factory)
{
    mComponentFactories.push_back(factory);
    return 1 << (mComponentFactories.size()-1);
}

ComponentFactory ComponentFactories::get(const TypeBits& typeBits)
{
    return mComponentFactories[typeBits];
}
