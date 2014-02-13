#include <Fission/Rendering/Transform.h>

namespace fsn
{
    Transform::Transform(sf::Vector2f pos, float rot, sf::Vector2f scale, const EntityRef& parent) : mParent(parent)
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

    void Transform::setParent(const EntityRef& parent)
    {
        mParent=parent;
    }

    sf::Transform Transform::getGlobalTransform()
    {
        sf::Transform transform = getTransform();
        for (EntityRef* parent = &mParent; parent->exists(); parent=&parent->getComponent<Transform>()->mParent)
        {
            transform = parent->getComponent<Transform>()->getTransform()*transform;
        }
        return transform;
    }
}

