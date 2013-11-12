#include <Fission/Core/Entity.h>

#include <Fission/Core/Scene.h>
#include <Fission/Core/Component.h>
#include <Fission/Core/EventManager.h>
#include <Fission/Core/EntityEvents.h>
#include <Fission/Core/ComponentFactories.h>

// By default, entities start with an ID of -1, marking them as invalid. When they are added to the
// scene, they will automatically be given an ID.
Entity::Entity(EventManager *eventManager) : mEventManager(eventManager), mID(-1), mTypeBits(0)
{
    //ctor
}

Entity::~Entity()
{
    // Release all of the components
    for (auto& components : mComponents)
        for (auto component : components)
            component->release();
}

void Entity::serialize(sf::Packet &packet) const
{
    packet << mID;
    packet << sf::Int32(mComponents.size()); // The component count

    for (auto& components : mComponents)
    {
        for (auto component : components)
        {
            packet << component->getTypeBits();
            component->serialize(packet);
        }
    }
}

void Entity::deserialize(sf::Packet &packet)
{
    packet >> mID;

    // Get all the components
    sf::Int32 componentCount;
    TypeBits componentType;

    packet >> componentCount;
    for (int c = 0; c < componentCount; c++)
    {
        packet >> componentType;

        auto factory = ComponentFactories::get(componentType);
        if (factory != NULL)
        {
            Component *component = factory();
            component->deserialize(packet);
            addComponent(component);
        }
        else
        {
            // TODO: Failed to deserialize Entity
            return;
        }
    }
}

void Entity::addComponent(Component *component)
{
    mTypeBits |= component->getTypeBits();
    int index = bitIndex(component->getTypeBits());
    if (index >= (int)mComponents.size())
        mComponents.resize(index+1);
    mComponents[index].push_back(component);

    mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, this, component));
}

Component* Entity::getComponent(TypeBits typeBits)
{
    int index = bitIndex(typeBits);
    if (index < int(mComponents.size()) && mComponents[index].size() > 0)
        return mComponents[index][0];
    return NULL;
}

std::vector<Component*> Entity::getComponents(TypeBits typeBits)
{
    std::vector<Component*> cmpnts;
	std::vector<int> indices = bitIndices(typeBits);
	for (int i : indices)
    {
		if (i < int(mComponents.size()))
            cmpnts.insert(cmpnts.end(), mComponents[i].begin(), mComponents[i].end());
	}
	return cmpnts;
}

Entity *Entity::clone() const
{
    sf::Packet packet;
    serialize(packet);
    Entity *clone = new Entity(mEventManager);
    clone->deserialize(packet);
    return clone;
}

int Entity::bitIndex(TypeBits val)
{
    int index = 0;
    while (val > 0)
    {
        val >>= 1;
        index++;
    }
    return index-1;
}

// bitIndices returns a slice of all the set bit indices
std::vector<int> Entity::bitIndices(TypeBits val)
{
    std::vector<int> bits;
    TypeBits shifter = 1;
    for (int i = 0; i < MAX_COMPONENT_TYPES; i++)
    {
        if ((val&shifter) != 0)
        {
            bits.push_back(i);
        }
        shifter <<= 1;
    }
    return bits;
}
