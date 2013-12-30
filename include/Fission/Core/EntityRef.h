#ifndef ENTITYREF_H
#define ENTITYREF_H

#include "Fission/Core/EntityManager.h"

class EntityRef
{
    friend class EntityManager;

    public:
        const static int NULL_ID = -1;

        /// \brief Add a component to this entity.
        template<typename component>
        void addComponent() const
        {
            mEntityManager->addComponentToEntity<component>(mID);
        }

        /// \brief Fast, unsafe way to get a component from this entity.
        template<typename component>
        component* getComponent() const
        {
            return mEntityManager->getComponentFromEntity<component>(mID);
        }

        /// \brief Fast, safe way to get a component from this entity. Use the unsafe version if
        /// you are certain that both this entity and the component exist.
        template<typename component>
        component* getComponentSafe() const
        {
            return mEntityManager->getComponentFromEntitySafe<component>(mID);
        }

        /// \brief Get the ID of the entity this points to.
        int getID() const {return mID;}

        /// \brief Get the bits for this entity
        const std::bitset<MAX_COMPONENTS>& getBits() const;

    private:
        EntityRef(EntityManager* em, int ID = NULL_ID); // Only EntityManager can instantiate

        EntityManager* mEntityManager;
        int mID; // The entity ID this reference points to
};

#endif // ENTITYREF_H
