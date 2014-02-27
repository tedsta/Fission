#ifndef ENTITYREF_H
#define ENTITYREF_H

#include <cstddef>

#include "Fission/Core/EntityManager.h"

namespace fsn
{
    class EntityRef
    {
        friend class EntityManager;

        public:
            static const int NullID = -1; // The 0th entity is the NULL entity.
            static const std::size_t NullUniqueID = 0;

            struct find : std::unary_function<EntityRef, bool>
            {
                int ID;
                find(int id) : ID(id) {}
                bool operator () (const EntityRef& e) const
                {
                    return e.mID == ID;
                }
            };

            /// \brief Default constructor. Creates EntityRef that points to the null entity
            EntityRef();

            /// \brief Check if this entity exists
            bool exists() const;

            /// \brief Destroy this entity.
            void destroy() const;

            /// \brief Serialize this entity.
            void serialize(Packet& packet) const;

            /// \brief Set this entity's tag
            void setTag(int tag) const;

            /// \brief Add a component to this entity.
            template<typename component, typename... Args>
            void addComponent(Args&&... args) const
            {
                if (mEntityManager)
                    mEntityManager->addComponentToEntity<component>(mID, std::forward<Args>(args)...);
            }

            /// \brief Fast, unsafe way to get a component from this entity.
            /// \return Corresponding component, or nullptr if it doesn't exist.
            template<typename component>
            inline component& getComponent() const
            {
                return mEntityManager->getComponentFromEntity<component>(mID);
            }

            /// \brief Fast, safe way to get a component from this entity. Use the unsafe version if
            /// you are certain that both this entity and the component exist.
            /// \return Corresponding component, or nullptr if it doesn't exist.
            template<typename component>
            component* getComponentSafe() const
            {
                if (mEntityManager)
                    return mEntityManager->getComponentFromEntitySafe<component>(mID);
                return nullptr;
            }

            /// \brief Get a component on an entity using the component's integer type ID.
            /// Useful for when you can't call the template method. Unsafe, so you have to be
            /// absolutely sure that the entity is valid
            /// \return Corresponding component, or nullptr if it doesn't exist.
            inline Component& getComponent(ComponentType componentID) const
            {
                return mEntityManager->getComponentFromEntity(mID, componentID);
            }

            /// \brief Get the ID of the entity this points to.
            int getID() const {return mID;}

            /// \brief Get this entity's unique ID
            std::size_t getUniqueID() const;

            /// \brief Get the bits for this entity
            const std::bitset<MaxComponents>& getBits() const;

            bool operator==(const EntityRef& other) const;
            bool operator!=(const EntityRef& other) const;

        private:
            EntityRef(EntityManager* em, int ID = NullID); // Only EntityManager can instantiate

            EntityManager* mEntityManager;
            int mID; // The entity ID this reference points to
    };
}


#endif // ENTITYREF_H
