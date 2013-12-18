#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <SFML/Network/Packet.hpp>

#include <Fission/Core/Component.h>

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

        /// \brief Create an entity and add it to the scene
        Entity* createEntity();

        /// \brief Add a game object to the scene.
        void addEntity(Entity* object);

        /// \brief Remove an entity from the scene. Does not delete entity.
        void removeEntity(Entity* entity);

        /// \brief Destroy an entity and remove it from the scene.
        void destroyEntity(Entity* entity);

        /// \brief Clear the scene.
        void clear();

    private:
        /// The event manager
        EventManager *mEventManager;

        /// The array of objects in this scene.
        std::vector<Entity*> mEntities;
};

#endif // SCENE_H
