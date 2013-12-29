#include "Fission/Core/Aspect.h"

#include "Fission/Core/EntityRef.h"

Aspect::Aspect()
{
    //ctor
}

Aspect::~Aspect()
{
    //dtor
}

bool Aspect::checkEntity(EntityRef* entity)
{
    std::bitset<MAX_COMPONENTS> entityBits = entity->getBits();
    if ((entityBits&mAll) == mAll && (mOne.none() || (entityBits&mOne).any()) && (entityBits&mExclude).none())
        return true;

    return false;
}
