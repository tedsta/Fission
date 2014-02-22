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
    std::unique_ptr<EntityManager> em(new EntityManager);
    int ID0 = em->createEntity();
    int ID1 = em->createEntity();
    CHECK(ID0 < ID1);
}

TEST(EntityManager_CreateEntityRef)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    int ID = em->createEntity();
    CHECK(em->createEntityRef(ID).getID() == ID);
}

TEST(EntityManager_CreateInvalidEntityRef)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    auto invalidRef = em->createEntityRef(1); // There are no entities yet, so this should be a NULL reference.
    CHECK(invalidRef.getID() == EntityRef::NullID);
}

TEST(EntityManager_DestroyEntity)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    auto entityID = em->createEntity();
    em->destroyEntity(entityID);
    CHECK(!em->entityExists(entityID));
}

TEST(EntityManager_AddComponentToEntity)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    auto entityID = em->createEntity();
    CHECK(em->getComponentFromEntitySafe<TestComponent>(entityID) == NULL);
    em->addComponentToEntity<TestComponent>(entityID);
    CHECK(em->getComponentFromEntitySafe<TestComponent>(entityID) != NULL);
}

TEST(EntityManager_RemoveComponentFromEntity)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    auto entityID = em->createEntity();
    em->addComponentToEntity<TestComponent>(entityID);
    em->removeComponentFromEntity<TestComponent>(entityID);
    CHECK(em->getComponentFromEntitySafe<TestComponent>(entityID) == NULL);
}

TEST(EntityManager_AddingComponentChangesEntityBits)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    auto entityID = em->createEntity();
    CHECK(!em->getEntityBits(entityID).test(1));
    em->addComponentToEntity<Test2Component>(entityID);
    CHECK(em->getEntityBits(entityID).test(Test2Component::Type()));
}

TEST(EntityManager_RemovingComponentChangesEntityBits)
{
    std::unique_ptr<EntityManager> em(new EntityManager);
    auto entityID = em->createEntity();
    em->addComponentToEntity<Test2Component>(entityID);
    em->removeComponentFromEntity<Test2Component>(entityID);
    CHECK(!em->getEntityBits(entityID).test(Test2Component::Type()));
}
