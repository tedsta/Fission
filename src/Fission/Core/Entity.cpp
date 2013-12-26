#include <Fission/Core/Entity.h>

#include <Fission/Core/Scene.h>
#include <Fission/Core/Component.h>
#include <Fission/Core/EventManager.h>
#include <Fission/Core/EntityEvents.h>
#include <Fission/Core/ComponentFactories.h>

std::vector<Entity*> Entity::Entities;
std::vector<int> Entity::FreeIDs;

Entity::Entity(EventManager *eventManager) : mEventManager(eventManager), mTypeBits(0), mComponentCount(0), mID(-1), mInScene(false)
{
}

Entity::~Entity()
{
    // Release all of the components
    for (auto& components : mComponents)
        for (auto component : components)
            component->release();

    if (mID != -1)
    {
        Entities[mID] = NULL;
        FreeIDs.push_back(mID);
    }

}

void Entity::giveID(int id)
{
    if (mID != -1)
        return;

    if (id == -1)
    {
        if (FreeIDs.size() == 0)
        {
            mID = Entities.size();
            Entities.push_back(this);
        }
        else
        {
            mID = FreeIDs.back();
            FreeIDs.pop_back();
        }

        for (auto& components : mComponents)
        {
            for (auto component : components)
            {
                component->giveID();
            }
        }
    }
    else if (id >= Entities.size())
    {
        mID = id;
        Entities.resize(mID+1);
        Entities[mID] = this;
    }
    else if (!Entities[id])
    {
        mID = id;
        Entities[mID] = this;
    }
}

void Entity::serialize(sf::Packet &packet) const
{
    packet << mID;
    packet << mComponentCount; // The component count

    for (auto& components : mComponents)
    {
        for (auto component : components)
        {
            packet << static_cast<int>(component->getTypeBits());
            packet << component->getID();
            component->serialize(packet);
        }
    }
}

void Entity::deserialize(sf::Packet &packet)
{
    int id;
    packet >> id;

    if (id != -1)
        giveID(id);

    // Get all the components
    int componentType;
    int componentCount;
    int componentID;

    packet >> componentCount;
    for (int c = 0; c < componentCount; c++)
    {
        packet >> componentType;
        packet >> componentID;

        auto factory = ComponentFactories::get(static_cast<TypeBits>(componentType));
        if (factory != NULL)
        {
            Component *component = factory();

            if (componentID != -1)
                component->giveID(componentID);

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

void Entity::postDeserialize()
{
    for (auto& components : mComponents)
    {
        for (auto component : components)
        {
            component->postDeserialize();
        }
    }
}

void Entity::addComponent(Component* component)
{
    if (mID != -1 && component->getID() == -1)
        component->giveID();

    mTypeBits |= component->getTypeBits();
    int index = bitIndex(component->getTypeBits());
    if (index >= (int)mComponents.size())
        mComponents.resize(index+1);
    mComponents[index].push_back(component);

    mComponentCount++;

    if (mInScene)
        mEventManager->fireEvent(EntityComponentEvent(EVENT_ADD_COMPONENT, this, component));
}

void Entity::addComponentSq(Sqrat::Object component)
{
    Component* _component = component.Cast<Component*>();
    _component->setSqInst(component);

    addComponent(_component);
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
