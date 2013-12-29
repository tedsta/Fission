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
            mBits |= ComponentTypeManager::getBit<component>();
        }

        const std::bitset<MAX_COMPONENTS>& getBits(){return mBits;}

    private:
        std::bitset<MAX_COMPONENTS> mBits;
};

TEST(Aspect_All)
{
    Aspect aspect;
    aspect.all<TestComponent, Test2Component>();

    std::unique_ptr<MockEntityRef> entity(new MockEntityRef);
    entity->addComponent<TestComponent>();
    CHECK(!aspect.checkEntity<MockEntityRef>(entity.get()));
    entity->addComponent<Test2Component>();
    CHECK(aspect.checkEntity<MockEntityRef>(entity.get()));
}

TEST(Aspect_One)
{
    Aspect aspect;
    aspect.one<TestComponent, Test2Component>();

    std::unique_ptr<MockEntityRef> entity(new MockEntityRef);
    CHECK(!aspect.checkEntity<MockEntityRef>(entity.get()));
    entity->addComponent<Test2Component>();
    CHECK(aspect.checkEntity<MockEntityRef>(entity.get()));
}

TEST(Aspect_Exclude)
{
    Aspect aspect;
    aspect.exclude<Test2Component>();

    std::unique_ptr<MockEntityRef> entity(new MockEntityRef);
    entity->addComponent<TestComponent>();
    CHECK(aspect.checkEntity<MockEntityRef>(entity.get()));
    entity->addComponent<Test2Component>();
    CHECK(!aspect.checkEntity<MockEntityRef>(entity.get()));
}
