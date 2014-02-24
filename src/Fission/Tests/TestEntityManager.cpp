#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"

#include "Fission/Tests/TestComponent.h"

using namespace fsn;

TEST(EntityManager_CreateEntityIncrementIDs)
{
    EntityManager em;
    int ID0 = em.createEntity();
    int ID1 = em.createEntity();
    CHECK(ID0 < ID1);
}

TEST(EntityManager_CreateEntityRef)
{
    EntityManager em;
    int ID = em.createEntity();
    CHECK(em.createEntityRef(ID).getID() == ID);
}

TEST(EntityManager_CreateInvalidEntityRef)
{
    EntityManager em;
    auto invalidRef = em.createEntityRef(1); // There are no entities yet, so this should be a NULL reference.
    CHECK(invalidRef.getID() == EntityRef::NullID);
}

TEST(EntityManager_DestroyEntity)
{
    EntityManager em;
    auto entityID = em.createEntity();
    em.destroyEntity(entityID);
    CHECK(!em.entityExists(entityID));
}

TEST(EntityManager_AddComponentToEntity)
{
    EntityManager em;
    auto entityID = em.createEntity();
    CHECK(em.getComponentFromEntitySafe<TestComponent>(entityID) == NULL);
    em.addComponentToEntity<TestComponent>(entityID);
    CHECK(em.getComponentFromEntitySafe<TestComponent>(entityID) != NULL);
}

TEST(EntityManager_RemoveComponentFromEntity)
{
    EntityManager em;
    auto entityID = em.createEntity();
    em.addComponentToEntity<TestComponent>(entityID);
    em.removeComponentFromEntity<TestComponent>(entityID);
    CHECK(em.getComponentFromEntitySafe<TestComponent>(entityID) == NULL);
}

TEST(EntityManager_AddingComponentChangesEntityBits)
{
    EntityManager em;
    auto entityID = em.createEntity();
    CHECK(!em.getEntityBits(entityID).test(1));
    em.addComponentToEntity<Test2Component>(entityID);
    CHECK(em.getEntityBits(entityID).test(Test2Component::Type()));
}

TEST(EntityManager_RemovingComponentChangesEntityBits)
{
    EntityManager em;
    auto entityID = em.createEntity();
    em.addComponentToEntity<Test2Component>(entityID);
    em.removeComponentFromEntity<Test2Component>(entityID);
    CHECK(!em.getEntityBits(entityID).test(Test2Component::Type()));
}
