#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <cstddef>

#include "Fission/Core/ComponentFactory.h"

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

            mComponents[ComponentFactory::getID<component>()-1][ID] = new component;
        }

        /// \brief Get a component on an entity.
        template<typename component>
        component* getComponentFromEntity(int ID)
        {
            return static_cast<component*>(mComponents[ComponentFactory::getID<component>()-1][ID]);
        }

        bool entityExists(int ID);

    private:
        IEventManager* mEventManager;
        std::vector<std::vector<Component*>> mComponents; // By type, by entity, respectively
        std::vector<int> mFreeIDs;
        int mNextID;
};

#endif // ENTITYMANAGER_H
