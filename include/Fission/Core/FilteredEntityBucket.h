#ifndef FISSION_FILTEREDENTITYBUCKET_H
#define FISSION_FILTEREDENTITYBUCKET_H

#include "Fission/Core/Aspect.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/EntityObserver.h"

namespace fsn
{
    class FilteredEntityBucket : public EntityObserver
    {
        public:
            FilteredEntityBucket();

            /// \brief Called when an entity is added to this bucket
            virtual void onEntityAdded(const EntityRef& entity){}

            /// \brief Called when an entity is removed from this bucket
            virtual void onEntityRemoved(const EntityRef& entity){}

            // Component requirement forwarding to aspect

            /// \brief Require all of these components.
            template<typename ...components>
            void all() {
                mAspect.all<components...>();
            }

            /// \brief Require at least one of these components.
            template<typename ...components>
            void one() {
                mAspect.one<components...>();
            }

            /// \brief Require that none of these components are present.
            template<typename ...components>
            void exclude() {
                mAspect.exclude<components...>();
            }

            // Getters

            /// \brief Get the active entities
            const std::vector<EntityRef>& getActiveEntities(){return mActiveEntities;}

        private:
            // Entity callbacks
            void onEntityCreated(const EntityRef& entity);
            void onEntityDestroyed(const EntityRef& entity);
            void onEntityAddedComponent(const EntityRef& entity, Component& component);
            void onEntityRemovedComponent(const EntityRef& entity, Component& component);

            // The aspect for this system. Determines which entities are processed by this system
            // based on their components.
            // TODO: Figure out a better way to modify the aspect of systems
            Aspect mAspect;

            /// \brief Checks if an entity should be processed by this system and adds or removes
            /// it from this system as necessary.
            void checkEntity(const EntityRef& entity);

            // The active entities this system should process.
            std::vector<EntityRef> mActiveEntities;
    };
}


#endif // FISSION_FILTEREDENTITYBUCKET_H
