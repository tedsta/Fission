#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporterStdout.h>

#include "Fission/Core/ComponentFactory.h"
#include "Fission/Tests/TestComponent.h"

int main(int, char const *[])
{
    ComponentFactory::registerComponent<TestComponent>();

    return UnitTest::RunAllTests();
}
