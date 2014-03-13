#include "Fission/Core/Systems/ComponentSystem.h"

namespace fsn
{
    ComponentSystem::ComponentSystem(EntityManager& entityMgr)
    {
        entityMgr.addEntityObserver(this);
    }

    void ComponentSystem::update(const float dt)
    {
        begin(dt);

        // Iterate through the active entities and process each one
        for (auto& entity : getActiveEntities())
        {
            processEntity(entity, dt);
        }

        end(dt);
    }
}
