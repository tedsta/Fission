#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/Network/Packet.hpp>

class Component;

/// \brief Type definition for component type IDs
typedef std::size_t ComponentType;

/// \brief Type definition for component factory functions.
typedef Component* (*ComponentFactory)();

/// \brief A tag class for components. Components must inherit from this.
class Component
{
    friend class ComponentTypeManager;

    public:
        virtual ~Component(){}

        /// \brief Get the component type ID of this component.
        virtual ComponentType getType() = 0;

    private:
        /// \brief A template component factory.
        template <typename c>
        static Component* factory()
        {
            return new c;
        }
};

#endif // COMPONENT_H
