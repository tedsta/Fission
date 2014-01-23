#include <Fission/Rendering/Transform.h>

namespace fsn
{
    Transform::Transform(sf::Vector2f pos, float rot, sf::Vector2f scale, Transform* parent) : mParent(parent)
    {
        setPosition(pos);
        setRotation(rot);
        setScale(scale);
    }

    Transform::~Transform()
    {
        //dtor
    }

    void Transform::serialize(sf::Packet& packet)
    {
        packet << getPosition().x << getPosition().y << getRotation() << getScale().x << getScale().y;
    }

    void Transform::deserialize(sf::Packet& packet)
    {
        sf::Vector2f position;
        float rotation;
        sf::Vector2f scale;
        packet >> position.x >> position.y >> rotation >> scale.x >> scale.y;
        setPosition(position);
        setRotation(rotation);
        setScale(scale);
    }

    sf::Transform Transform::getGlobalTransform()
    {
        sf::Transform transform = getTransform();
        for (Transform* parent = mParent; parent != nullptr; parent=parent->mParent)
        {
            transform.combine(parent->getTransform());
        }
        return transform;
    }
}

