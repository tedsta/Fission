#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"
#include "Fission/Core/Aspect.h"

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

class MockEntityRef
{
    public:
        template<typename component>
        void addComponent()
        {
            mBits |= ComponentFactory::getBit<component>();
        }

        const std::bitset<MAX_COMPONENTS>& getBits(){return mBits;}

    private:
        std::bitset<MAX_COMPONENTS> mBits;
};

TEST(Aspect_All)
{
    Aspect all;
    all.all<TestComponent, Test2Component>();

    std::unique_ptr<MockEntityRef> entity(new MockEntityRef);
    entity->addComponent<TestComponent>();
    CHECK(!all.checkEntity<MockEntityRef>(entity.get()));
    entity->addComponent<Test2Component>();
    CHECK(all.checkEntity<MockEntityRef>(entity.get()));
}
