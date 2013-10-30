#include "Rendering/TransformComponent.h"

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
