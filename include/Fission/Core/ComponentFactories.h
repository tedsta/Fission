#ifndef COMPONENTFACTORIES_H
#define COMPONENTFACTORIES_H

#include <Fission/Core/Component.h>

/// \brief Type definition for component factory functions.
typedef Component *(*ComponentFactory)();

class ComponentFactories
{
    public:
        /// \brief Register a new component factory function.
        static TypeBits add(ComponentFactory factory);

        /// \brief Get a component factory function by name.
        static ComponentFactory get(const TypeBits& typeBits);

    private:
        /// The component creation functions
        static std::vector<ComponentFactory> mComponentFactories;
};

#endif // COMPONENTFACTORIES_H
