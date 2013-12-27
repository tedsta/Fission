#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <set>
#include <vector>

class Component;
class EntityRef;

class EntityManager
{
    friend class EntityRef;

    public:
        EntityManager();
        virtual ~EntityManager();

        /// \brief Creates a new entity.
        /// \return EntityRef to new entity.
        EntityRef* createEntity();

        /// \brief Creates a new entity entity reference to an existing entity.
        EntityRef* createEntityRef(int ID);

    private:
        std::vector<std::vector<Component*>> mComponents; // By type, by entity, respectively
        std::set<int> mFreeIDs;
        int mNextID;
};

#endif // ENTITYMANAGER_H
