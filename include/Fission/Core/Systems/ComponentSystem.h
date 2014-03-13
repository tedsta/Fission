#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include <vector>

#include "Fission/Core/Aspect.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/FilteredEntityBucket.h"
#include "Fission/Core/Systems/System.h"

namespace fsn
{
    class ComponentSystem : public System, public FilteredEntityBucket
    {
        public:
            ComponentSystem(EntityManager& entityMgr);

            /// \brief Called each time before processing any entities.
            virtual void begin(const float dt) {}

            /// \brief Process a single entity.
            virtual void processEntity(const EntityRef& entity, const float dt) = 0;

            /// \brief Called each time after processing all of the entities.
            virtual void end(const float dt) {}

            /// \brief Engine uses this internally to update the system.
            void update(const float dt);

        private:
    };
}

#endif // COMPONENTSYSTEM_H
