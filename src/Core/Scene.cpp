#include "Core/Scene.h"

#include <iostream>
#include <fstream>

#include "Core/System.h"
#include "Core/Entity.h"
#include "Core/EventManager.h"
#include "Core/EntityEvents.h"

Scene::Scene(EventManager *eventManager) : mEventManager(eventManager), mNextID(0)
{
    //ctor
}

Scene::~Scene()
{
    // Clear the scene when it's destroyed
    clear();
}

void Scene::save(std::string fileName)
{
    std::ofstream file(fileName.c_str(), std::ios::out|std::ios::binary);

    sf::Packet packet;
    serialize(packet);

    file.write((const char*)packet.getData(), packet.getDataSize());

    file.close();
}

void Scene::load(std::string fileName)
{
    std::ifstream file(fileName.c_str(), std::ios::in|std::ios::binary);

    sf::Packet packet;

    int size = file.tellg();
    file.seekg(0, std::ios::end);
    size = ((int)file.tellg()) - size;
    file.seekg(0, std::ios::beg);

    // Create the buffer and read file contents into it
    char *buffer = new char[size];
    file.read(buffer, size);
    packet.append(buffer, size);
    delete[] buffer;

    deserialize(packet);

    packet.clear();
    file.close();
}

void Scene::serialize(sf::Packet &packet)
{
    packet << sf::Int32(mEntities.size());
    for (Entity *entity : mEntities)
    {
        entity->serialize(packet);
    }
}

void Scene::deserialize(sf::Packet &packet)
{
    int entityCount;
    packet >> entityCount;

    for (int i = 0; i < entityCount; i++)
    {
        Entity *entity = new Entity(mEventManager);
        entity->deserialize(packet);
    }
}

Entity* Scene::createEntity()
{
    Entity* e = new Entity(mEventManager);
    addEntity(e);
    return e;
}

void Scene::addEntity(Entity *entity)
{
    entity->mID = mNextID;
    mNextID++; // Increment the ID for the next entity
    mEntities.push_back(entity);

    mEventManager->fireEvent(EntityEvent(EVENT_ADD_ENTITY, entity));
}

void Scene::destroyEntity(Entity *entity)
{
    for (auto it = mEntities.begin(); it != mEntities.end(); it++)
    {
        if ((*it) == entity)
        {
            delete *it;
            mEntities.erase(it);
            return;
        }
    }
}

Entity *Scene::findEntity(int ID) const
{
    for (Entity* entity : mEntities)
    {
        if (entity->getID() == ID) //find the Entity
            return entity;
    }

    return NULL; // Couldn't find it
}

void Scene::clear()
{
    for (Entity *entity : mEntities)
        entity->release(); //release the Entity
    mEntities.clear();
}
