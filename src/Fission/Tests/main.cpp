#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporterStdout.h>

#include "Fission/Core/ComponentTypeManager.h"
#include "Fission/Tests/TestComponent.h"

using namespace fission;

int main(int, char const *[])
{
    ComponentTypeManager::add<TestComponent>();
    ComponentTypeManager::add<Test2Component>();

    return UnitTest::RunAllTests();
}
