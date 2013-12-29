#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

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

    private:
        IEventManager* mEventManager;
        std::vector<std::vector<Component*>> mComponents; // By type, by entity, respectively
        std::vector<int> mFreeIDs;
        int mNextID;
};

#endif // ENTITYMANAGER_H
