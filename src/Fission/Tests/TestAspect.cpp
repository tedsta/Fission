#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"
#include "Fission/Core/Aspect.h"

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

		bool fireEvent(IEventData const& evt)
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
            mBits |= ComponentTypeManager::getBit<component>();
        }

        const std::bitset<MaxComponents>& getBits() const {return mBits;}

    private:
        std::bitset<MaxComponents> mBits;
};

TEST(Aspect_All)
{
    Aspect aspect;
    aspect.all<TestComponent, Test2Component>();

    MockEntityRef entity;
    entity.addComponent<TestComponent>();
    CHECK(!aspect.checkEntity<MockEntityRef>(entity));
    entity.addComponent<Test2Component>();
    CHECK(aspect.checkEntity<MockEntityRef>(entity));
}

TEST(Aspect_One)
{
    Aspect aspect;
    aspect.one<TestComponent, Test2Component>();

    MockEntityRef entity;
    CHECK(!aspect.checkEntity<MockEntityRef>(entity));
    entity.addComponent<Test2Component>();
    CHECK(aspect.checkEntity<MockEntityRef>(entity));
}

TEST(Aspect_Exclude)
{
    Aspect aspect;
    aspect.exclude<Test2Component>();

    MockEntityRef entity;
    entity.addComponent<TestComponent>();
    CHECK(aspect.checkEntity<MockEntityRef>(entity));
    entity.addComponent<Test2Component>();
    CHECK(!aspect.checkEntity<MockEntityRef>(entity));
}
