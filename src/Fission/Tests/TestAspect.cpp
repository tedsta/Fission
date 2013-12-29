#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"
#include "Fission/Core/Aspect.h"

#include "Fission/Tests/Defs.h"
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

TEST(Aspect_All)
{
    std::unique_ptr<IEventManager> eventManager(new MockEventManager);
    std::unique_ptr<EntityManager> em(new EntityManager(eventManager.get()));

    Aspect all;
    all.all<TestComponent, Test2Component>();

    auto entity = em->createEntity();
    em->addComponentToEntity<TestComponent>(entity->getID());
    CHECK(!all.checkEntity(entity));
    em->addComponentToEntity<Test2Component>(entity->getID());
    CHECK(all.checkEntity(entity));
}
