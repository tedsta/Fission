#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <cstddef>

#include "Fission/Core/ComponentTypeManager.h"
#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityEvents.h"

namespace fsn
{
    class IEventManager;
    class Component;
    class EntityRef;

    class EntityManager
    {
        friend class ComponentMapper;

        public:
            EntityManager(IEventManager* eventManager);
            virtual ~EntityManager();

            /// \brief Creates a new entity.
            /// \return ID of new entity.
            int createEntity();

            /// \brief Creates a new entity entity reference to an existing entity.
            EntityRef* getEntityRef(int ID);

            /// \brief Destroy an existing entity.
            void destroyEntity(int ID);

            /// \brief Add a component to an entity.
            template<typename component>
            void addComponentToEntity(int ID)
            {
                if (!entityExists(ID))
                    return;

                if (component::Type() >= mComponents.size()) // Our component table's type dimension isn't big enough yet.
                {
                    mComponents.resize(component::Type()+1);
                    for (auto& componentRow : mComponents)
                        componentRow.resize(mNextID, NULL);
                }

                mComponents[component::Type()][ID] = new component; // Create the new component
                mEntityBits[ID] |= ComponentTypeManager::getBit<component>(); // Add the component's bit to the entity's bits.

                // Tell the world the component's been added
                mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, getEntityRef(ID), mComponents[component::Type()][ID]));
            }

            /// \brief Add a component to an entity.
            void addComponentToEntity(int ID, Component* component)
            {
                if (!entityExists(ID))
                    return;

                if (component->getType() >= mComponents.size()) // Our component table's type dimension isn't big enough yet.
                {
                    mComponents.resize(component->getType()+1);
                    for (auto& componentRow : mComponents)
                        componentRow.resize(mNextID, NULL);
                }

                mComponents[component->getType()][ID] = component; // Create the new component
                mEntityBits[ID] |= ComponentTypeManager::getBit(component->getType()); // Add the component's bit to the entity's bits.

                // Tell the world the component's been added
                mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, getEntityRef(ID), mComponents[component->getType()][ID]));
            }

            /// \brief Remove a component from an entity.
            template<typename component>
            void removeComponentFromEntity(int ID)
            {
                if (!entityExists(ID))
                    return;

                if (component::Type() >= mComponents.size()) // No entities have this component
                    return;

                if (!mComponents[component::Type()][ID]) // This entity doesn't have this component
                    return;

                delete mComponents[component::Type()][ID]; // Delete the component
                mComponents[component::Type()][ID] = NULL;
                mEntityBits[ID] &= ComponentTypeManager::getBit<component>().flip(); // Remove the component's bit from the entity's bits.

                // Tell the world that this component's been removed from this entity.
                mEventManager->fireEvent(EntityComponentEvent(EVENT_REMOVE_COMPONENT, getEntityRef(ID), mComponents[component::Type()][ID]));
            }

            /// \brief Fastest, unsafe way to get a component on an entity.
            /// \note Template is for casting convenience
            template<typename component>
            component* getComponentFromEntity(int ID) const
            {
                return static_cast<component*>(mComponents[component::Type()][ID]);
            }

            /// \brief Get a component on an entity using the component's integer type ID.
            /// Useful for when you can't call the template method
            Component* getComponentFromEntity(int ID, ComponentType componentID) const
            {
                return mComponents[componentID][ID];
            }

            /// \brief Fast, safe way to get a component on an entity. Use the unsafe version if you
            /// are certain both the entity and the component exist.
            /// \note Template is for casting convenience
            template<typename component>
            component* getComponentFromEntitySafe(int ID) const
            {
                if (!entityExists(ID))
                    return NULL;

                if (static_cast<std::size_t>(component::Type()) < mComponents.size())
                    return static_cast<component*>(mComponents[component::Type()][ID]);

                return NULL;
            }

            /// \brief Get entity bits.
            const std::bitset<MaxComponents>& getEntityBits(int ID) const {return mEntityBits[ID];}

            /// \brief Get the number of active entities.
            int getEntityCount() const {return mEntityCount;}

            /// \brief Get whether or not an entity exists.
            bool entityExists(int ID) const;

        private:
            IEventManager* mEventManager;
            std::vector<std::vector<Component*>> mComponents; // By component type, by entity ID.
            std::vector<std::bitset<MaxComponents>> mEntityBits; // By entity ID
            std::vector<EntityRef*> mEntityRefs; // Store all of the entity refs
            int mEntityCount; // Total number of active entities

            std::vector<int> mFreeIDs;
            int mNextID; // Entity IDs start at 1. The 0th entity is the NULL entity.
    };
}

#endif // ENTITYMANAGER_H
