#include "Fission/Core/EntityRef.h"

namespace fission
{
    EntityRef::EntityRef(EntityManager* em, int ID) : mEntityManager(em), mID(ID)
    {
        //ctor
    }

    const std::bitset<MaxComponents>& EntityRef::getBits() const
    {
        return mEntityManager->getEntityBits(mID);
    }
}

