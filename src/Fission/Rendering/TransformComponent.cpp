#include <Fission/Rendering/TransformComponent.h>

TypeBits TransformComponent::Type;

TransformComponent::TransformComponent(sf::Vector2f pos, float rot, sf::Vector2f scale)
{
    setPosition(pos);
    setRotation(rot);
    setScale(scale);
}

TransformComponent::~TransformComponent()
{
    //dtor
}

void TransformComponent::serialize(sf::Packet& packet)
{
    packet << getPosition().x << getPosition().y << getRotation() << getScale().x << getScale().y;
}

void TransformComponent::deserialize(sf::Packet& packet)
{
    sf::Vector2f position;
    float rotation;
    sf::Vector2f scale;
    packet >> position.x >> position.y >> rotation >> scale.x >> scale.y;
    setPosition(position);
    setRotation(rotation);
    setScale(scale);
}
