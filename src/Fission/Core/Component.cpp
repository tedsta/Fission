#include <Fission/Core/Component.h>

std::vector<Component*> Component::Components;
std::vector<int> Component::FreeIDs;

Component::Component() : mID(-1)
{
}

Component::~Component()
{
    if (mID != -1)
    {
        Components[mID] = NULL;
        FreeIDs.push_back(mID);
    }
}

void Component::giveID(int id)
{
    if (mID != -1)
        return;

    if (id == -1)
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
    else if (id >= Components.size())
    {
        mID = id;
        Components.resize(mID+1);
        Components[mID] = this;
    }
    else if (!Components[id])
    {
        mID = id;
        Components[mID] = this;
    }
}
