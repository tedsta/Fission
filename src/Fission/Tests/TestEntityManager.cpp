#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"

#include "Fission/Tests/TestComponent.h"

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
    auto ref0 = em->createEntity();
    auto ref1 = em->createEntity();
    CHECK(ref0->getID() == 0);
    CHECK(ref1->getID() == 1);
}

TEST(EntityManager_CreateEntityRef)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    em->createEntity();
    CHECK(em->createEntityRef(0)->getID() == 0);
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
    auto entity = em->createEntity();
    em->destroyEntity(entity->getID());
    CHECK(!em->entityExists(entity->getID()));
}

TEST(EntityManager_AddComponentToEntity)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entity = em->createEntity();
    CHECK(em->getComponentFromEntity<TestComponent>(entity->getID()) == NULL);
    em->addComponentToEntity<TestComponent>(entity->getID());
    CHECK(em->getComponentFromEntity<TestComponent>(entity->getID()) != NULL);
}

TEST(EntityManager_AddingComponentChangesEntityBits)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));
    auto entity = em->createEntity();
    CHECK(!em->getEntityBits(entity->getID()).test(1));
    em->addComponentToEntity<Test2Component>(entity->getID());
    CHECK(em->getEntityBits(entity->getID()).test(1));
}
