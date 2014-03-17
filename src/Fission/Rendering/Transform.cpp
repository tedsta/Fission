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

    void Transform::serialize(Packet& packet)
    {
        packet << getPosition().x << getPosition().y << getRotation() << getScale().x << getScale().y << getOrigin().x << getOrigin().y;
    }

    void Transform::deserialize(Packet& packet)
    {
        sf::Vector2f position;
        float rotation;
        sf::Vector2f scale;
        sf::Vector2f origin;

        packet >> position.x >> position.y >> rotation >> scale.x >> scale.y >> origin.x >> origin.y;

        setPosition(position);
        setRotation(rotation);
        setScale(scale);
        setOrigin(origin);
    }

    void Transform::setParent(const EntityRef& parent)
    {
        mParent=parent;
    }

    sf::Transform Transform::getGlobalTransform()
    {
        sf::Transform transform = getTransform();
        for (EntityRef* parent = &mParent; parent->exists(); parent=&parent->getComponent<Transform>().mParent)
        {
            transform = parent->getComponent<Transform>().getTransform()*transform;
        }
        return transform;
    }
}

