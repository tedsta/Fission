#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"

TEST(EntityManager_CreateEntity)
{
    std::unique_ptr<EntityManager> em(new EntityManager());
    auto ref0 = em->createEntity();
    auto ref1 = em->createEntity();
    CHECK(ref0->getID() == 0);
    CHECK(ref1->getID() == 1);
}

TEST(EntityManager_CreateEntityRef)
{
    std::unique_ptr<EntityManager> em(new EntityManager());
    em->createEntity();
    CHECK(em->createEntityRef(0)->getID() == 0);
}

TEST(EntityManager_CreateInvalidEntityRef)
{
    std::unique_ptr<EntityManager> em(new EntityManager());
    auto invalidRef = em->createEntityRef(0); // There are no entities yet, so this should be a NULL reference.
    CHECK(invalidRef->getID() == EntityRef::NULL_ID);
}
