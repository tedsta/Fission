#include "Fission/Core/ComponentType.h"

ComponentType::ComponentType()
{
    init();
}

void ComponentType::init()
{
    bit = nextBit;
    nextBit = nextBit << 1;
    ID = nextID++;
}

std::bitset<MAX_COMPONENTS> ComponentType::getBit() const
{
    return bit;
}

int ComponentType::getID() const
{
    return ID;
}

std::bitset<MAX_COMPONENTS> ComponentType::nextBit(1);
int ComponentType::nextID = 1;
