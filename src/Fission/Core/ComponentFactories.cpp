#include <Fission/Core/ComponentFactories.h>

#include <Fission/Core/Entity.h>

std::vector<ComponentFactory> ComponentFactories::mComponentFactories;

TypeBits ComponentFactories::add(ComponentFactory factory)
{
    mComponentFactories.push_back(factory);
    return 1 << (mComponentFactories.size()-1);
}

ComponentFactory ComponentFactories::get(const TypeBits& typeBits)
{
    int bits = typeBits;
    int index = 0;
    while (bits > 0)
    {
        bits >>= 1;
        index++;
    }
    index--;

    if (index < mComponentFactories.size())
        return mComponentFactories[index];
    return NULL;
}
