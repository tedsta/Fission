#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <bitset>

#include <SFML/Network/Packet.hpp>

#include <Fission/Core/config.h>
#include <Fission/Core/RefCounted.h>
#include <Fission/Core/Component.h>

class EventManager;
class Scene;

class Entity : public RefCounted
{
    friend class Scene;

    public:
        Entity(EventManager *eventManager);
        virtual ~Entity();

        /// \brief Serialize this entity.
        void serialize(sf::Packet &packet) const;

        /// \brief Deserialize this entity.
        void deserialize(sf::Packet &packet);

        /// \brief Adds a new component to this entity.
        void addComponent(Component *component);

        // Getters

        /// \brief Get the ID of this entity.
        int getID() const {return mID;}

        /// \brief Get the type bits of this entity.
        const TypeBits& getTypeBits() const {return mTypeBits;}

        /// \brief Get single component by type bits.
        Component* getComponent(TypeBits typeBits);

        /// \brief Get components by type bits.
        std::vector<Component*> getComponents(TypeBits typeBits);

        /// \brief Creates a clone of this entity.
        Entity *clone() const;

    private:
        /// The event manager.
        EventManager *mEventManager;

        /// The unique ID of this entity.
        int mID;

        /// The type bits of this entity.
        TypeBits mTypeBits;

        /// The components attached to this entity.
        std::vector<std::vector<Component*>> mComponents;

        // ****************************************************************************************

        // bitIndex returns the index of a single set bit in an integer
        int bitIndex(TypeBits val);

        // bitIndices returns an array of all the set bit indices
        std::vector<int> bitIndices(TypeBits val);
};

#endif // ENTITY_H
