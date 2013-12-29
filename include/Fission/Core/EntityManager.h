#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <cstddef>

#include "Fission/Core/ComponentTypeManager.h"
#include "Fission/Core/EventManager.h"
#include "Fission/Core/EntityEvents.h"

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
        EntityRef* createEntityRef(int ID);

        /// \brief Destroy an existing entity.
        void destroyEntity(int ID);

        /// \brief Add a component to an entity.
        template<typename component>
        void addComponentToEntity(int ID)
        {
            if (!entityExists(ID))
                return;

            const ComponentType& type = ComponentTypeManager::getTypeFor<component>();

            if (static_cast<std::size_t>(type.getID()) > mComponents.size()) // Our component table's type dimension isn't big enough yet.
            {
                mComponents.resize(type.getID());
                for (auto& componentRow : mComponents)
                    componentRow.resize(mNextID, NULL);
            }

            mComponents[type.getID()-1][ID] = new component; // Create the new component
            mEntityBits[ID] |= type.getBit(); // Add the component's bit to the entity's bits.

            // Tell the world the component's been added
            mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, createEntityRef(ID), mComponents[type.getID()-1][ID]));
        }

        /// \brief Remove a component from an entity.
        template<typename component>
        void removeComponentFromEntity(int ID)
        {
            if (!entityExists(ID))
                return;

            const ComponentType& type = ComponentTypeManager::getTypeFor<component>();

            if (static_cast<std::size_t>(type.getID()) > mComponents.size()) // No entities have this component
                return;

            if (!mComponents[type.getID()-1][ID]) // This entity doesn't have this component
                return;

            delete mComponents[type.getID()-1][ID]; // Delete the component
            mComponents[type.getID()-1][ID] = NULL;
            mEntityBits[ID] &= type.getBit().flip(); // Remove the component's bit from the entity's bits.

            // Tell the world that this component's been removed from this entity.
            mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, createEntityRef(ID), mComponents[type.getID()-1][ID]));
        }

        /// \brief Slow, convenient way to get a component on an entity. Do not use in code that is executed
        /// every frame. In those areas, use the overloaded method and supply the component ID.
        template<typename component>
        component* getComponentFromEntity(int ID) const
        {
            if (!entityExists(ID))
                return NULL;

            const ComponentType& type = ComponentTypeManager::getTypeFor<component>();

            if (static_cast<std::size_t>(type.getID()) <= mComponents.size())
                return static_cast<component*>(mComponents[type.getID()-1][ID]);

            return NULL;
        }

        /// \brief Fastest, unsafe way to get a component on an entity.
        /// \note Template is for casting convenience
        template<typename component = Component>
        component* getComponentFromEntity(int ID, int componentID) const
        {
            return static_cast<component*>(mComponents[componentID-1][ID]);
        }

        /// \brief Fast, safe way to get a component on an entity. Use the unsafe version if you
        /// are certain both the entity and the component exist.
        /// \note Template is for casting convenience
        template<typename component = Component>
        component* getComponentFromEntitySafe(int ID, int componentID) const
        {
            if (!entityExists(ID))
                return NULL;

            if (static_cast<std::size_t>(componentID) <= mComponents.size())
                return static_cast<component*>(mComponents[componentID-1][ID]);

            return NULL;
        }

        /// \brief Get entity bits.
        const std::bitset<MAX_COMPONENTS>& getEntityBits(int ID) const {return mEntityBits[ID];}

        /// \brief Get the number of active entities.
        int getEntityCount() const {return mEntityCount;}

        /// \brief Get whether or not an entity exists.
        bool entityExists(int ID) const;

    private:
        IEventManager* mEventManager;
        std::vector<std::vector<Component*>> mComponents; // By component type, by entity ID.
        std::vector<std::bitset<MAX_COMPONENTS>> mEntityBits; // By entity ID
        int mEntityCount; // Total number of active entities

        std::vector<int> mFreeIDs;
        int mNextID;
};

#endif // ENTITYMANAGER_H
