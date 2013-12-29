#include "Fission/Core/EntityRef.h"

EntityRef::EntityRef(EntityManager* em, int ID) : mEntityManager(em), mID(ID)
{
    //ctor
}

const std::bitset<MAX_COMPONENTS>& EntityRef::getBits() const
{
    return mEntityManager->getEntityBits(mID);
}
