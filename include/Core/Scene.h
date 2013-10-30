#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <SFML/Network/Packet.hpp>

#include "Core/Component.h"

class EventManager;
class Entity;

/// \brief A scene in the game.
class Scene
{
    public:
        Scene(EventManager *eventManager);
        virtual ~Scene();

        /// \brief Save the scene to a file.
        void save(std::string filename);

        /// \brief Load a scene from a file.
        void load(std::string filename);

        /// \brief Serialize the scene.
        void serialize(sf::Packet &packet);

        /// \brief Deserialize the scene.
        void deserialize(sf::Packet &packet);

        /// \brief Add a game object to the scene.
        void addEntity(Entity *object);

        /// \brief Destroy a game object and remove it from the scene.
        void destroyEntity(Entity *object);

        /// \brief Find a game object by ID.
        Entity *findEntity(int ID) const;

        /// \brief Clear the scene.
        void clear();

    private:
        /// The event manager
        EventManager *mEventManager;

        /// The array of objects in this scene.
        std::vector<Entity*> mEntities;

        /// The next object's ID
        int mNextID;
};

#endif // SCENE_H
