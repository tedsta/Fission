#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"
#include "Fission/Core/Aspect.h"

class TestComponent : public Component
{
    public:
        TestComponent() : mData(0)
        {
        }

        int mData;
};

TEST(Aspect_All)
{
    Aspect aspect;
    aspect.all<TestComponent>();
}
