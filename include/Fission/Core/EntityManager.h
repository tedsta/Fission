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
    friend class EntityRef;

    public:
        EntityManager(IEventManager* eventManager);
        virtual ~EntityManager();

        /// \brief Creates a new entity.
        /// \return EntityRef to new entity.
        EntityRef* createEntity();

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

            if (static_cast<std::size_t>(type.getID()) > mComponents.size())
            {
                mComponents.resize(type.getID());
                for (auto& componentRow : mComponents)
                    componentRow.resize(mNextID, NULL);
            }

            mComponents[type.getID()-1][ID] = new component;
            mEntityBits[ID] |= type.getBit();

            mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, createEntityRef(ID), mComponents[type.getID()-1][ID]));
        }

        /// \brief Get a component on an entity.
        template<typename component>
        component* getComponentFromEntity(int ID) const
        {
            if (!entityExists(ID))
                return NULL;

            const ComponentType& type = ComponentTypeManager::getTypeFor<component>();

            if (static_cast<std::size_t>(type.getID()) <= mComponents.size())
                return static_cast<component*>(mComponents[ComponentTypeManager::getID<component>()-1][ID]);

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
