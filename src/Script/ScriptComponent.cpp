#include "Script/ScriptComponent.h"

#include <iostream>

TypeBits ScriptComponent::Type;

ScriptComponent::ScriptComponent(HSQUIRRELVM vm)
{
    mUpdateFunction = Sqrat::RootTable(vm).GetFunction("update");
}

ScriptComponent::~ScriptComponent()
{
    //sq_close(mUpdateFunction.GetVM());
}

void ScriptComponent::executeUpdate(const float dt)
{
    if (!mUpdateFunction.IsNull())
    {
        mUpdateFunction.Execute(dt);
    }
}
