#ifndef ENTITYREF_H
#define ENTITYREF_H

class EntityManager;

class EntityRef
{
    friend class EntityManager;

    public:
        const static int NULL_ID = -1;

        virtual ~EntityRef();

        /// \brief Get the ID of the entity this points to.
        int getID(){return mID;}

    private:
        EntityRef(EntityManager* em, int ID = NULL_ID); // Only EntityManager can instantiate

        EntityManager* mEntityManager;
        int mID; // The entity ID this reference points to
};

#endif // ENTITYREF_H
