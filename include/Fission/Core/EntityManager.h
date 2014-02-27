#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <memory>

#include "Fission/Core/ComponentTypeManager.h"
#include "Fission/Core/EventManager.h"
#include "Fission/Core/IEntityObserver.h"
#include "Fission/Core/Packet.h"

#include "Fission/Util/make_unique.h"

namespace fsn
{
    class IEventManager;
    class Component;
    class EntityRef;

    class EntityManager
    {
        friend class Engine;
        friend class ComponentMapper;

        public:
            EntityManager();
            virtual ~EntityManager();

            /// \brief Creates a new entity.
            /// \param giveUniqueID Set to true if the new entity needs a unique ID. Useful for
            /// networking.
            /// \return ID of new entity.
            int createEntity(bool giveUniqueID = false);

            /// \brief Creates a new entity reference to an existing entity.
            EntityRef createEntityRef(int ID);

            /// \brief Get entity ID from an unique entity ID
            int getEntityIDFromUniqueID(std::size_t uniqueID);

            /// \brief Destroy an existing entity.
            void destroyEntity(int ID);

            /// \brief Serialize an entity.
            void serializeEntity(int ID, Packet& packet);

            /// \brief Creates a new entity from a packet.
            /// \return ID of new entity
            int deserializeEntity(Packet& packet);

            /// \brief Lock entity destruction. All destroyed entities will be removed when it is unlocked.
            void lockEntityDestruction();

            /// \brief Unlock entity destruction. Any entities destroyed while locked will now be removed.
            void unlockEntityDestruction();

            /// \brief Set an entity's tag
            void setEntityTag(int ID, int tag);

            /// \brief Add a component to an entity.
            template<typename component, typename... Args>
            void addComponentToEntity(int ID, Args&&... args)
            {
                if (!entityExists(ID))
                    return;

                if (component::Type() >= mComponents.size()) // Our component table's type dimension isn't big enough yet.
                {
                    mComponents.resize(component::Type()+1);
                    for (auto& componentRow : mComponents)
                        componentRow.resize(mNextID);
                }

                mComponents[component::Type()][ID] = make_unique<component>(std::forward<Args>(args)...); // Create the new component
                mEntityBits[ID] |= ComponentTypeManager::getBit(component::Type()); // Add the component's bit to the entity's bits.

                // Tell the world the component's been added
                for (auto observer : mObservers)
                    observer->onEntityAddedComponent(createEntityRef(ID), *mComponents[component::Type()][ID]);
            }

            /// \brief Add a component to an entity with specified ComponentType.
            template<typename... Args>
            void addComponentToEntity(int ID, ComponentType type, Args&&... args)
            {
                if (!entityExists(ID))
                    return;

                if (type >= mComponents.size()) // Our component table's type dimension isn't big enough yet.
                {
                    mComponents.resize(type+1);
                    for (auto& componentRow : mComponents)
                        componentRow.resize(mNextID);
                }

                mComponents[type][ID] = std::unique_ptr<Component>(ComponentTypeManager::createComponent(type)); // Create the new component
                mEntityBits[ID] |= ComponentTypeManager::getBit(type); // Add the component's bit to the entity's bits.

                // Tell the world the component's been added
                for (auto observer : mObservers)
                    observer->onEntityAddedComponent(createEntityRef(ID), *mComponents[type][ID]);
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

                // Tell the world that this component's been removed from this entity.
                for (auto observer : mObservers)
                    observer->onEntityRemovedComponent(createEntityRef(ID), *mComponents[component::Type()][ID]);

                mComponents[component::Type()][ID].reset(); // Delete the component
                mEntityBits[ID] &= ComponentTypeManager::getBit<component>().flip(); // Remove the component's bit from the entity's bits.
            }

            /// \brief Fastest, unsafe way to get a component on an entity.
            /// \note Template is for casting convenience
            template<typename component>
            inline component& getComponentFromEntity(int ID) const
            {
                return *static_cast<component*>(mComponents[component::Type()][ID].get());
            }

            /// \brief Get a component on an entity using the component's integer type ID.
            /// Useful for when you can't call the template method
            inline Component& getComponentFromEntity(int ID, ComponentType componentID) const
            {
                return *mComponents[componentID][ID];
            }

            /// \brief Fast, safe way to get a component on an entity. Use the unsafe version if you
            /// are certain both the entity and the component exist.
            /// \note Template is for casting convenience
            template<typename component>
            component* getComponentFromEntitySafe(int ID) const
            {
                if (!entityExists(ID))
                    return nullptr;

                if (static_cast<std::size_t>(component::Type()) < mComponents.size())
                    return static_cast<component*>(mComponents[component::Type()][ID].get());

                return nullptr;
            }

            /// \brief Get entity's unique ID
            std::size_t getUniqueEntityID(int ID) const {return mUniqueIDs[ID];}

            /// \brief Get entity bits.
            const std::bitset<MaxComponents>& getEntityBits(int ID) const {return mEntityBits[ID];}

            /// \brief Get the tag of an entity
            int getEntityTag(int ID) const {return mEntityTags[ID];}

            /// \brief Get the array of entity's with a tag
            const std::vector<EntityRef>& getEntitiesWithTag(int tag);

            /// \brief Get the number of active entities.
            int getEntityCount() const {return mEntityCount;}

            /// \brief Get whether or not an entity exists.
            bool entityExists(int ID) const;

            /// \brief Add an entity observer.
            void addEntityObserver(IEntityObserver* observer){mObservers.push_back(observer);}

        private:
            void removeEntitiesMarkedForRemoval();

            std::vector<std::size_t> mUniqueIDs; // Unique entity IDs
            std::vector<std::vector<std::unique_ptr<Component>>> mComponents; // By component type, by entity ID.
            std::vector<std::bitset<MaxComponents>> mEntityBits; // By entity ID
            std::vector<int> mEntityTags; // Entity tags
            std::vector<int> mEntitiesToRemove; // Entities to remove before the next frame
            std::vector<std::vector<EntityRef>> mTaggedEntities; // All of the tagged entities
            int mEntityCount; // Total number of active entities

            std::vector<IEntityObserver*> mObservers; // Entity listeners

            bool mDestructionLocked;

            std::vector<int> mFreeIDs;
            int mNextID; // Entity IDs start at 0
            std::size_t mNextUniqueID; // Unique entity IDs start at 1, 0 is invalid
    };
}

#endif // ENTITYMANAGER_H
