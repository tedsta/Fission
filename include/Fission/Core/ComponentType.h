#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <bitset>
#include "Fission/Core/config.h"

/// \brief Identifies a bitset and ID for a component object
/// Do not instantiate a ComponentType, instead use the ComponentTypeManager.
class ComponentType
{
    public:
        ComponentType();

        std::bitset<MAX_COMPONENTS> getBit() const;
        int getID() const;

    private:
        static std::bitset<MAX_COMPONENTS> nextBit;
        static int nextID;

        std::bitset<MAX_COMPONENTS> bit;
        int ID;
        void init();
};

#endif // COMPONENTTYPE_H
