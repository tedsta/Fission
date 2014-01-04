#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"

#include "Fission/Tests/TestComponent.h"

using namespace fsn;

class MockEventManager : public IEventManager
{
    public:
		void addListener(IEventListener *listener, EventID type)
		{
		}

		void removeListener(IEventListener *listener, EventID type)
		{
		}

		void addGlobalListener(IEventListener *listener)
		{
		}

		void removeGlobalListener(IEventListener *listener)
		{
		}

		void removeAllListeners()
		{
		}

		bool fireEvent(IEventData const& evt) const
		{
		    return false;
		}
};

TEST(EntityManager_CreateEntityIncrementIDs)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    int ID0 = em->createEntity();
    int ID1 = em->createEntity();
    CHECK(ID0 == 0);
    CHECK(ID1 == 1);
}

TEST(EntityManager_CreateEntityRef)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    int ID = em->createEntity();
    CHECK(em->createEntityRef(ID)->getID() == ID);
}

TEST(EntityManager_CreateInvalidEntityRef)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto invalidRef = em->createEntityRef(0); // There are no entities yet, so this should be a NULL reference.
    CHECK(invalidRef->getID() == EntityRef::NULL_ID);
}

TEST(EntityManager_DestroyEntity)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entityID = em->createEntity();
    em->destroyEntity(entityID);
    CHECK(!em->entityExists(entityID));
}

TEST(EntityManager_AddComponentToEntity)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entityID = em->createEntity();
    CHECK(em->getComponentFromEntitySafe<TestComponent>(entityID) == NULL);
    em->addComponentToEntity<TestComponent>(entityID);
    CHECK(em->getComponentFromEntitySafe<TestComponent>(entityID) != NULL);
}

TEST(EntityManager_RemoveComponentFromEntity)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entityID = em->createEntity();
    em->addComponentToEntity<TestComponent>(entityID);
    em->removeComponentFromEntity<TestComponent>(entityID);
    CHECK(em->getComponentFromEntitySafe<TestComponent>(entityID) == NULL);
}

TEST(EntityManager_AddingComponentChangesEntityBits)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entityID = em->createEntity();
    CHECK(!em->getEntityBits(entityID).test(1));
    em->addComponentToEntity<Test2Component>(entityID);
    CHECK(em->getEntityBits(entityID).test(Test2Component::Type()));
}

TEST(EntityManager_RemovingComponentChangesEntityBits)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entityID = em->createEntity();
    em->addComponentToEntity<Test2Component>(entityID);
    em->removeComponentFromEntity<Test2Component>(entityID);
    CHECK(!em->getEntityBits(entityID).test(Test2Component::Type()));
}
