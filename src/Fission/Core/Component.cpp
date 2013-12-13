#include <Fission/Core/Component.h>

std::vector<Component*> Component::Components;
std::vector<int> Component::FreeIDs;

Component::Component()
{
    if (FreeIDs.size() == 0)
    {
        mID = Components.size();
        Components.push_back(this);
    }
    else
    {
        mID = FreeIDs.back();
        FreeIDs.pop_back();
    }
}

Component::~Component()
{
    Components[mID] = NULL;
    FreeIDs.push_back(mID);
}
