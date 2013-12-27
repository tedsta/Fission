#include "Fission/Core/EntityRef.h"

EntityRef::EntityRef(EntityManager* em, int ID) : mEntityManager(em), mID(ID)
{
    //ctor
}

EntityRef::~EntityRef()
{
    //dtor
}
